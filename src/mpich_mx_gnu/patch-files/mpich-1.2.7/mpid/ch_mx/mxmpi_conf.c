/*************************************************************************
 * Myricom MPICH-MX ch_mx backend                                        *
 * Copyright (c) 2003 by Myricom, Inc.                                   *
 * All rights reserved.                                                  *
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#ifdef WIN32
#define strcasecmp stricmp
#endif

#define MX_STRONG_TYPES 0

#include "mxmpi.h"
#if !defined MX_API || MX_API < 0x300
#define MX_GET_INFO_IN_PARAMS
#else
#define MX_GET_INFO_IN_PARAMS NULL,0,
#endif

#include "mpid.h"
#include "bnr.h"
#include "mpiddev.h"
#if 0
#include "mpimem.h"
#endif

#include "mxmpi_smpi.h"
#include "mxmpi_inline.h"

#if MX_INTERP
FILE *interp_file = NULL;
#endif


static void 
mxmpi_getenv (const char *varenv, char **result, char *msg, 
	     unsigned int required)
{
  *result = (char *) getenv (varenv);
  if ((required == 1) && (*result == NULL))
    {
      fprintf (stderr, "<MPICH-MX> Error: Need to obtain %s in %s !\n",
	       msg, varenv);
      mxmpi_abort (0);
    }
}


static void
mxmpi_allocate_endpoint (int nic_index)
{
  mx_return_t rc;
  uint32_t nic_count;
  uint64_t *nic_ids;
  /* do we know which NIC to use ? */
  if (nic_index == -1)
    { 
      rc = mx_open_endpoint(MX_ANY_NIC,
	  MX_ANY_ENDPOINT,
	  mxmpi.filter_value,
	  NULL, 0,
	  &mxmpi.my_endpoint);
      if (rc == MX_SUCCESS)
	{
	  return;
	}
    }
  else
    {
      rc = mx_get_info(NULL, MX_NIC_COUNT, MX_GET_INFO_IN_PARAMS &nic_count, sizeof (nic_count));
      if ((rc == MX_SUCCESS) && (nic_index < nic_count)) {
	nic_ids = malloc((nic_count+1) * sizeof (*nic_ids));
	if (nic_ids != NULL) {
	  rc = mx_get_info(NULL, MX_NIC_IDS, MX_GET_INFO_IN_PARAMS nic_ids, (nic_count+1) * sizeof (*nic_ids));
	  if (rc == MX_SUCCESS) {
	    rc = mx_open_endpoint(
#if MX_API +0 >= 0x200
				  nic_index,
#else
				  nic_ids[nic_index],
#endif
				  MX_ANY_ENDPOINT,
				  mxmpi.filter_value,
				  NULL, 0,
				  &mxmpi.my_endpoint);
	    if (rc == MX_SUCCESS)
	      {
		free (nic_ids);
		return;
	      }
	  }
	  free (nic_ids);
	}
      }
    }

  fprintf(stderr, "[%d] Error: mx_open_endpoint() failed: %s\n",
          MPID_MyWorldRank, mx_strerror(rc));
  mxmpi_abort(0);
}


static void
mxmpi_allocate_world (unsigned int size)
{
  mxmpi.addrs = calloc (size, sizeof (*mxmpi.addrs));
  mxmpi.mpi_pids = (unsigned int *) calloc (size, sizeof (*mxmpi.mpi_pids));
  mxmpi.host_names = (char **) calloc (size, sizeof (char *));
  mxmpi.exec_names = (char **) calloc (size, sizeof (char *));
  
  mxmpi_malloc_assert (mxmpi.addrs, "mxmpi_getconf", "malloc: addresses");
  mxmpi_malloc_assert (mxmpi.mpi_pids, "mxmpi_getconf", "malloc: mpi_pids");
  mxmpi_malloc_assert (mxmpi.host_names, "mxmpi_getconf", "malloc: host_names");
  mxmpi_malloc_assert (mxmpi.exec_names, "mxmpi_getconf", "malloc: exec_names");
}

#define GMPI_INIT_TIMEOUT MXMPI_INIT_TIMEOUT
#define gmpi_abort mxmpi_abort
#include "gmpi_tcpinit.c"

#define MXMPI_SOCKET_BUFFER_SIZE 128*1024*10


static void
mxmpi_getconf (void)
{
  char *mxmpi_eager, *mxmpi_shmem, *mxmpi_recvmode;
  uint32_t endpoint_id;
  unsigned int i, j;
  int nic_index, k;
  uint64_t nic_id;
  mx_return_t rc;
  extern int errno;
  
  setbuf (stdout, NULL);
  setbuf (stderr, NULL);

#if 0
  if (getenv ("MAN_MPD_FD") != NULL)
    {
      char attr_buffer[BNR_MAXATTRLEN];
      char val_buffer[BNR_MAXVALLEN];
      /* MPD to spawn processes */
      char my_hostname[256];
      char *hostnames;
      BNR_Group bnr_group;
      
      /* MPD to spawn processes */
      mxmpi.mpd = 1;
      i = BNR_Init ();
      i = BNR_Get_group (&bnr_group);
      i = BNR_Get_rank (bnr_group, &MPID_MyWorldRank);
      i = BNR_Get_size (bnr_group, &MPID_MyWorldSize);
      
      /* data allocation */
      mxmpi_allocate_world (MPID_MyWorldSize);
      hostnames = (char *) malloc (MPID_MyWorldSize * 256 * sizeof (char *));
      mxmpi_malloc_assert (hostnames, "mxmpi_getconf", "malloc: hostnames");

      /* open a port */
      nic_index = -1;

      if (mxmpi_allocate_endpoint (nic_index) != 0) {
	fprintf (stderr, "[%d] Error: Unable to open a MX port !\n", 
		 MPID_MyWorldRank);
	mxmpi_abort (0);
      }
      
      /* get my MX endpoint address */
  
      /* build the data to send to master */
      bzero (val_buffer, BNR_MAXVALLEN * sizeof (char));
      gethostname(my_hostname, sizeof (my_hostname));
      sprintf (val_buffer, "< %s:%s >\n", 
	       mx_endpoint_to_text(mxmpi.my_endpoint), my_hostname);
      
      /* put our information */
      bzero (attr_buffer, BNR_MAXATTRLEN * sizeof (char));
      sprintf (attr_buffer, "MPICH-MX data [%d]\n", MPID_MyWorldRank);
      i = BNR_Put (bnr_group, attr_buffer, val_buffer, -1);
      
      /* get other processes data */
      i = BNR_Fence (bnr_group);
      for (j = 0; j < MPID_MyWorldSize; j++)
	{
	  bzero (attr_buffer, BNR_MAXATTRLEN * sizeof (char));
	  sprintf (attr_buffer, "MPICH-MX data [%d]\n", j);
	  i = BNR_Get (bnr_group, attr_buffer, val_buffer);
	  
	  /* decrypt data */
	  if (sscanf (val_buffer, "< %s:%s >",
		      ep_buf,
		      &(hostnames[j*256])) != 2)
	    {
	      fprintf (stderr, "[%d] Error: unable to decode data "
		       "from %d !\n", MPID_MyWorldRank, j);
	      mxmpi_abort (0);
	    }
	  mxmpi.endpoints[j] = mx_text_to_endpoint(ep_buf);
	}
      
      /* compute the local mapping */
      smpi.num_local_nodes = 0;
      for (j = 0; j < MPID_MyWorldSize; j++)
	{
	  if (strcmp (my_hostname, &(hostnames[j*256])) == 0)
	    {
	      if (j == MPID_MyWorldRank)
		{
		  smpi.my_local_id = smpi.num_local_nodes;
		}
	      smpi.local_nodes[j] = smpi.num_local_nodes;
	      smpi.num_local_nodes++;
	    }
	  else
	    {
	      smpi.local_nodes[j] = -1;
	    }
	}
      free (hostnames);
    }
  else
#endif
    {
      char *mxmpi_magic_str, *mxmpi_master, *mxmpi_port;
      char *mxmpi_id, *mxmpi_np, *mxmpi_nic, *mxmpi_file;
      char buffer[MXMPI_SOCKET_BUFFER_SIZE];
      char *bp;
      char temp[128];
      uint32_t count, master_port, mxmpi_magic, board_num;
      
      /* mpirun with sockets */
      mxmpi.mpd = 0;
      gethostname(temp, sizeof (temp)-1);
      mxmpi_getenv ("MXMPI_MAGIC", &mxmpi_magic_str, "the job magic number", 1);
      mxmpi_getenv ("MXMPI_MASTER", &mxmpi_master, "the master's hostname", 1);
      mxmpi_getenv ("MXMPI_PORT", &mxmpi_port, "the master's port number", 1);
      mxmpi_getenv ("MXMPI_ID", &mxmpi_id, "the MPI ID of the process", 1);
      mxmpi_getenv ("MXMPI_NP", &mxmpi_np, "the number of MPI processes", 1);
      mxmpi_getenv ("MXMPI_BOARD", &mxmpi_nic, "the specified nic", 1);
      mxmpi_getenv ("MXMPI_FILE", &mxmpi_file, NULL, 0);
      
      if (sscanf (mxmpi_magic_str, "%d", &mxmpi_magic) != 1)
	{
	  fprintf (stderr, "<MPICH-MX> Error on %s: Bad magic number "
		   "(MXMPI_MAGIC is %s) !\n", temp, mxmpi_magic_str);
	  mxmpi_abort (0);
	}

      /* convert magic number into a filter value */
      mxmpi.filter_value = mxmpi_magic;
      
      if ((sscanf (mxmpi_np, "%d", &MPID_MyWorldSize) != 1)
	  || (MPID_MyWorldSize < 0))
	{
	  fprintf (stderr, "<MPICH-MX> Error on %s: Bad number of processes "
		   "(MXMPI_NP is %s) !\n", temp, mxmpi_np);
	  mxmpi_abort (0);
	}
      
      if ((sscanf (mxmpi_id, "%d", &MPID_MyWorldRank) != 1)
	  || (MPID_MyWorldRank < 0) || (MPID_MyWorldRank >= MPID_MyWorldSize))
	{
	  fprintf (stderr, "<MPICH-MX> Error on %s: Bad MPI ID "
		   "(MXMPI_ID is %s, total number of MPI processes is %d) !\n", 
		   temp, mxmpi_np, MPID_MyWorldSize);
	  mxmpi_abort (0);
	}
  
      if (sscanf (mxmpi_port, "%d", &master_port) != 1)
	{
	  fprintf (stderr, "<MPICH-MX> Error on %s: Bad master port  number "
		   "(MXMPI_PORT is %s) !\n", temp, mxmpi_port);
	  mxmpi_abort (0);
	}

      if (sscanf (mxmpi_nic, "%d", &nic_index) != 1)
	{
	  fprintf (stderr, "<MPICH-MX> Error on %s: Bad nic index "
		   "(MXMPI_NIC is %s) !\n", temp, mxmpi_nic);
	  mxmpi_abort (0);
	}

      /* data allocation */
      mxmpi_allocate_world (MPID_MyWorldSize);

      if (!mxmpi_file) {
	
	/* open a port */
	endpoint_id = 0;
	mxmpi_allocate_endpoint (nic_index);
	if (mxmpi.my_endpoint == NULL)
	  {
	    fprintf (stderr, "[%d] Error: Unable to open a MX port !\n", 
		     MPID_MyWorldRank);
	    mxmpi_abort (0);
	  }
	
	/* get my MX endpoint address */
	if (mx_get_endpoint_addr(mxmpi.my_endpoint, &mxmpi.my_endpoint_addr)
	    != MX_SUCCESS)
	  {
	    fprintf (stderr, "[%d] Error: Unable to get MX local endpoint !\n", 
		     MPID_MyWorldRank);
	    mxmpi_abort (0);
	  }
	
	mx_decompose_endpoint_addr(mxmpi.my_endpoint_addr,
				   &nic_id, &endpoint_id);
	mx_nic_id_to_board_number(nic_id, &board_num);
	
#if MX_INTERP
	{
	  char fname[256];
	  if (getenv("MXMPI_INTERP_PREFIX") != NULL) {
	    sprintf(fname, "%s%d", getenv("MXMPI_INTERP_PREFIX"), MPID_MyWorldRank);
	    interp_file = fopen(fname, "w");
	    if (interp_file == NULL) {
	      fprintf(stderr, "Cannot open %s for interpetor output!\n", fname);
	    } else {
	      fprintf(interp_file, "mx_interp -v << EEE\n");
	      fprintf(interp_file, "open e1 any %d 0x%x\n",
		      endpoint_id, mxmpi.filter_value);
	    }
	  }
	}
#endif
	gmpi_tcpinit(buffer, MXMPI_SOCKET_BUFFER_SIZE,
		     mxmpi_master, master_port, &mxmpi.master_addr,
		     mxmpi_magic, MPID_MyWorldRank,
		     endpoint_id + (board_num << 16),
		     MX_U32(nic_id), MX_L32(nic_id),
		     0,
		     (int) getpid ());
	
	bp = buffer;
	if (strncmp (bp, "[[[", 3) != 0)
	  {
	    fprintf (stderr, "[%d] Error: bad format on data from master !\n",
		     MPID_MyWorldRank);
	    mxmpi_abort (0);
	  }
	
	/* Decrypt the global mapping */
	bp += 3;
	for (i = 0; i < MPID_MyWorldSize; i++)
	  {
	    uint32_t port_board_id;
	    uint16_t ep_id;
	    uint32_t nic_id_hi;
	    uint32_t nic_id_lo;
	    uint64_t his_nic_id;
	    uint32_t his_pid;
	    char *hostname;
	    char *hend;
	    
	    if (sscanf (bp, "<%d:%u:%u:%u>",
			&port_board_id, &nic_id_hi, &nic_id_lo, &his_pid) != 4)
	      {
		fprintf (stderr, "[%d] Error: unable to decode data "
			 "from master !\n", MPID_MyWorldRank);
		mxmpi_abort (0);
	      }
	    
	    mxmpi.mpi_pids[i] = his_pid;
	    his_nic_id = ((uint64_t) nic_id_hi << 32) | nic_id_lo;
	    ep_id = port_board_id & 0xffff;
	    rc = (i == MPID_MyWorldRank) ?
	      mx_get_endpoint_addr(mxmpi.my_endpoint, &mxmpi.addrs[i].addr) :
	      mx_connect(mxmpi.my_endpoint, his_nic_id, ep_id, mxmpi.filter_value,
			 MX_INFINITE, &mxmpi.addrs[i].addr);
	    mxmpi.host_names[i] = malloc(MX_MAX_HOSTNAME_LEN + 1);
	    if (mxmpi.host_names[i] == NULL) {
	      fprintf(stderr, "[%d] malloc hostname: \"%s\"\n",
		      MPID_MyWorldRank,
		      strerror(errno));
	      mxmpi_abort(0);
	    }
	    mx_nic_id_to_hostname(his_nic_id, mxmpi.host_names[i]);
	    
	    MXMPI_NOTE(("addr[%d] = id=%d, nic=%llx, filter=%x, hostname=%s\n",
			i, ep_id, his_nic_id, filter, hostname));
	    
#if MX_INTERP
	    if (interp_file != NULL) {
	      fprintf(interp_file, "target t%d %s:2 0 %d 0x%x\n",
		      i, mxmpi.host_names[i], ep_id, filter);
	      fflush(interp_file);
	    }
#endif
	    bp = strchr(bp,'>') + 1;
	  }
#if MX_INTERP
	if (interp_file != NULL) {
	  fprintf(interp_file, "sleep 5\n");
	  fflush(interp_file);
	}
#endif
	
	/* check the marker between global map and local map */  
	if (strncmp (bp, "|||", 3) != 0)
	  {
	    fprintf (stderr, "[%d] Error: bad format 2 on data from master !\n",
		     MPID_MyWorldRank);
	    mxmpi_abort (0);
	  }
	
	/* decrypt the local mapping */
	bp += 3;
	smpi.num_local_nodes = 0;
	while (strncmp (bp, "]]]", 3) != 0)
	  { 
	    if (sscanf (bp, "<%d>", &i) != 1)
	      {
		fprintf (stderr, "[%d] Error: unable to decode master data !\n",
			 MPID_MyWorldRank);
		mxmpi_abort (0);
	      }
	    
	    if (i == MPID_MyWorldRank)
	      {
		smpi.my_local_id = smpi.num_local_nodes;
	      }
	    smpi.num_local_nodes++;
	    
	    bp = strchr(bp, '>');
	    if (bp == NULL) {
	      fprintf(stderr, "[%d] Error: bad fmt 4 on data from master\n",
		      MPID_MyWorldRank);
	      mxmpi_abort(0);
	    }
	    ++bp;
	  }
	
	/* check size of the data from the master */
	bp += 3;
      } else {
	uint32_t bnum;
      /* read the configuration from a file */
	FILE *f = fopen(mxmpi_file,"r");
	int nb_host;
	if (!f) {
	  perror(mxmpi_file);
	  exit(1);
	}
	for (nb_host=0;nb_host < MPID_MyWorldSize;nb_host++) {
	  char *h = malloc(256);
	  int dst_bnum, dst_eid;
	  char l[256];
	  if (!fgets(l,256, f))
	    break;
	  /* remove end-of-line */
	  l[strlen(l) - 1] = 0;
	  switch (sscanf(l,"%s %d %d", h, &dst_bnum, &dst_eid)) {
	  case 1:
	    dst_bnum = 0;
	  case 2:
	    dst_eid = 0;
	  case 3:
	    break;
	  default:
	    fprintf(stderr,"syntax error in file %s:line %d:%s\n",mxmpi_file, nb_host,l);
	    exit(1);
	  }
	  mxmpi.host_names[nb_host] = h;
	  sprintf(h + strlen(h), ":%d", dst_bnum);
	  mxmpi.addrs[nb_host].eid = dst_eid;
	}
	assert(nb_host > 0);
	if (ferror(f)) {
	  perror(mxmpi_file);
	  exit(1);
	}
	fclose(f);
	for (i=0;i<MPID_MyWorldSize;i++) {
	  uint64_t dst_nic_id;
	  uint32_t eid;
	  mxmpi.addrs[i].eid = mxmpi.addrs[i % nb_host].eid + 1;
	  mxmpi.host_names[i] = mxmpi.host_names[i % nb_host];
	}
	if (1) {
	  fprintf(stderr,"Rank %d on host %s eid = %d\n",
		  MPID_MyWorldRank, mxmpi.host_names[MPID_MyWorldRank], 
		  mxmpi.addrs[MPID_MyWorldRank].eid);
	}
	mx_hostname_to_nic_id(mxmpi.host_names[MPID_MyWorldRank], &nic_id);
	mx_nic_id_to_board_number(nic_id, &bnum);
	mx_open_endpoint(bnum, mxmpi.addrs[MPID_MyWorldRank].eid, mxmpi.filter_value,
			 NULL, 0, &mxmpi.my_endpoint);
	for (i=0;i<MPID_MyWorldSize;i++) {
	  uint64_t dst_nic_id;
	  int j;
	  int rc;
	  mx_hostname_to_nic_id(mxmpi.host_names[i], &dst_nic_id);
	  
	  mx_set_error_handler(MX_ERRORS_RETURN);
	  for (j=0;j<10;j++) {
	    if ((rc = mx_connect(mxmpi.my_endpoint, dst_nic_id, mxmpi.addrs[i].eid, 
				 mxmpi.filter_value, MX_INFINITE, &mxmpi.addrs[i].addr)) == MX_SUCCESS)
	      goto connected;
	    sleep(1);
	  }
	  fprintf(stderr, "Cannot connect to %s/%d (%"PRIx64"): %s\n",
		  mxmpi.host_names[i], mxmpi.addrs[i].eid, dst_nic_id, mx_strerror(rc));
	  exit(1);
	connected:
	  mx_set_error_handler(MX_ERRORS_ARE_FATAL);
	}
      }
    }
  
  /* Stolen and modified from below. */
  mxmpi_getenv ("MXMPI_RECV", &mxmpi_recvmode, NULL, 0);

  /* set the MX receive mode */
  /* reese XXX - make mx_receive_mode a flag */
  if (mxmpi_recvmode == NULL)
    {
      mxmpi.test_or_wait = &mx_test;
    }
  else
    {
      if (strcasecmp (mxmpi_recvmode, "polling") == 0)
	{
	  mxmpi.test_or_wait = &mx_test;
	}
      else
	{
	  if (strcasecmp (mxmpi_recvmode, "blocking") == 0)
	    {
	      mxmpi.test_or_wait = &mxmpi_wait_wrapper;
	    }
	  else
	    {
	      mxmpi.test_or_wait = &mx_test;
	    }
	}
    }

  /* check consistency */
#if 0
  if ((mxmpi.port_ids[MPID_MyWorldRank] != port_id) 
      || (mxmpi.nic_ids[MPID_MyWorldRank] != nic_id)
      || (mxmpi.addrs[MPID_MyWorldRank] != mxmpi.my_endpoint_addr))
    {
      fprintf (stderr, "[%d] Error: inconsistency in collected data !\n", 
	       MPID_MyWorldRank);
      mxmpi_abort (0);
    }

  mxmpi_getenv ("MXMPI_EAGER", &mxmpi_eager, NULL, 0);
  mxmpi_getenv ("MXMPI_SHMEM", &mxmpi_shmem, NULL, 0);
  mxmpi_getenv ("MXMPI_RECV", &mxmpi_recvmode, NULL, 0);

  /* Set the EAGER/Rendez-vous protocols threshold */
  if (mxmpi_eager == NULL)
    {
      mxmpi.eager_size = MXMPI_EAGER_SIZE_DEFAULT;
    }
  else
    {
      mxmpi.eager_size = strtoul(mxmpi_eager, NULL, 10);
      if (mxmpi.eager_size < 128)
	{
	  mxmpi.eager_size = 128;
	}
      
      if (mxmpi.eager_size > MXMPI_MAX_PUT_LENGTH)
	{
	  mxmpi.eager_size = MXMPI_MAX_PUT_LENGTH;
	}
    }

  /* set the MX receive mode */
  /* reese XXX - make mx_receive_mode a flag */
  if (mxmpi_recvmode == NULL)
    {
      mxmpi.mx_receive_mode = MXMPI_RXMODE_POLLING;
    }
  else
    {
      if (strcasecmp (mxmpi_recvmode, "polling") == 0)
	{
	  mxmpi.mx_receive_mode = MXMPI_RXMODE_POLLING;
	}
      else
	{
	  if (strcasecmp (mxmpi_recvmode, "blocking") == 0)
	    {
	      mxmpi.mx_receive_mode = MXMPI_RXMODE_BLOCKING;
	    }
	  else
	    {
	      if (strcasecmp (mxmpi_recvmode, "hybrid") == 0)
		{
		  mxmpi.mx_receive_mode = MXMPI_RXMODE_BLOCKING;
		}
	      else
		{
		  mxmpi.mx_receive_mode = MXMPI_RXMODE_POLLING;
		}
	    }
	}
    }

  /* Set the shared memory support */
  if (mxmpi.mx_receive_mode == MXMPI_RXMODE_POLLING)
    {
      if (mxmpi_shmem == NULL)
	{
	  mxmpi.shmem = 1;
	}
      else
	{
	  if (strcmp (mxmpi_shmem, "1") == 0) 
	    {
	      mxmpi.shmem = 1;
	    }
	  else
	    {
	      mxmpi.shmem = 0;
	    }
	}
    }
  else
    {
      mxmpi.shmem = 0;
    }
#endif
}


/* This function fill the MPID_Config structure to describe 
   the multi-devices configuration */
MPID_Config *MPID_GetConfigInfo (int *argc, 
				 char ***argv, 
				 void *config, 
				 int  *error_code)
{
  MPID_Config * new_config = NULL;
  MPID_Config * return_config = NULL;
  int i, j;

  /* already configured ? */
  if (config != NULL)
      return (MPID_Config *)config;

  /* Get the MX mapping and the environnement variables */
  mxmpi_getconf();


#if 0
  /* SELF DEVICE */
  /* if it's the first device, start the linked list of devices.
     Otherwise, add a new one at the end. */
  if (new_config == NULL)
    {
      new_config = (MPID_Config *)malloc(sizeof(MPID_Config));
      mxmpi_malloc_assert(new_config,
			 "MPID_GetConfigInfo",
			 "malloc: Self device config");
      return_config = new_config;
    }
  else
    {
      new_config->next = (MPID_Config *)malloc(sizeof(MPID_Config));
      mxmpi_malloc_assert(new_config->next,
			 "MPID_GetConfigInfo",
			 "malloc: Self device config");
      new_config = new_config->next;
    }
  /* we don't need to check if we need this device: we need this device ! */
  new_config->device_init = MPID_CH_InitSelfMsg;
  new_config->device_init_name = (char *)malloc(255*sizeof(char));
  mxmpi_malloc_assert(new_config->device_init_name,
		     "MPID_GetConfigInfo",
		     "malloc: Self device name");
  sprintf(new_config->device_init_name, "Self device");
  new_config->num_served = 1;
  new_config->granks_served = (int *)malloc(sizeof(int));
  mxmpi_malloc_assert(new_config->granks_served,
		     "MPID_GetConfigInfo",
		     "malloc: Self device map");
  new_config->granks_served[0] = MPID_MyWorldRank;
  new_config->next = NULL;
  
  
  /* SMP DEVICE */
  /* we don't need this device if there's only one process on this node */
  if (smpi.num_local_nodes > 1) 
    {
#if !MX_OS_VXWORKS
      if (mxmpi.shmem == 1)
	{
	  if (mxmpi.mx_receive_mode == mx_receive) 
	    {
	      if (new_config == NULL) 
		{
		  new_config = (MPID_Config *)malloc(sizeof(MPID_Config));
		  mxmpi_malloc_assert(new_config,
				     "MPID_GetConfigInfo",
				     "malloc: SMP device config");
		  return_config = new_config;
		}
	      else 
		{
		  new_config->next = (MPID_Config *)
		    malloc(sizeof(MPID_Config));
		  mxmpi_malloc_assert(new_config->next,
				     "MPID_GetConfigInfo",
				     "malloc: SMP device config");
		  new_config = new_config->next;
		}
	      new_config->device_init = MPID_SMP_InitMsgPass;
	      new_config->device_init_name = (char *)
		malloc(255 * sizeof(char));
	      mxmpi_malloc_assert(new_config->device_init_name,
				 "MPID_GetConfigInfo",
				 "malloc: SMP device name");
	      sprintf(new_config->device_init_name, "SMP_plug device");
	      new_config->num_served = smpi.num_local_nodes - 1;
	      new_config->granks_served = (int *)
		malloc(new_config->num_served * sizeof(int));
	      mxmpi_malloc_assert(new_config->granks_served,
				 "MPID_GetConfigInfo",
				 "malloc: SMP device map");
	      /* setup routes */
	      j = 0;
	      for(i=0; i<MPID_MyWorldSize; i++)
		{
		  if ((i!= MPID_MyWorldRank) 
		      && (smpi.local_nodes[i] != -1)) 
		    {
		      mxmpi_debug_assert(smpi.local_nodes[i] 
					!= smpi.my_local_id);
		      new_config->granks_served[j] = i;
		      j++;
		    }
		}
	      mxmpi_debug_assert(j == new_config->num_served);
	      new_config->next = NULL;
	    }
	}
      else
#endif
	{
	  smpi.num_local_nodes = 1;
	  for (i=0; i < MPID_MyWorldSize; i++)
	    {
	      if (i != MPID_MyWorldRank)
		{
		  smpi.local_nodes[i] = -1;
		}
	    }
	}
    }
  

  /* MX DEVICE */
  if (MPID_MyWorldSize > smpi.num_local_nodes)
    {
      if (new_config == NULL)
	{
	  new_config = (MPID_Config *)malloc(sizeof(MPID_Config));
	  mxmpi_malloc_assert(new_config,
			     "MPID_GetConfigInfo",
			     "malloc: MX device config");
	  return_config = new_config;
	}
      else
	{
	  new_config->next = (MPID_Config *)malloc(sizeof(MPID_Config));
	  mxmpi_malloc_assert(new_config->next,
			     "MPID_GetConfigInfo",
			     "malloc: MX device config");
	  new_config = new_config->next;
	}
      new_config->device_init = MPID_CH_InitMsgPass;
      new_config->device_init_name = (char *)malloc(255 * sizeof(char));
      mxmpi_malloc_assert(new_config->device_init_name,
			 "MPID_GetConfigInfo",
			 "malloc: MX device name");
      sprintf(new_config->device_init_name, "Myricom MX device");
      new_config->num_served = MPID_MyWorldSize - smpi.num_local_nodes;
      new_config->granks_served = (int *)malloc(new_config->num_served 
						* sizeof(int));
      mxmpi_malloc_assert(new_config->granks_served,
			 "MPID_GetConfigInfo",
			 "malloc: MX device map");
      /* setup the routes */
      j = 0;
      for(i=0; i<MPID_MyWorldSize; i++)
	if (smpi.local_nodes[i] == -1) {
	  new_config->granks_served[j] = i;
	  j++;
	}
      mxmpi_debug_assert(j == new_config->num_served);
      new_config->next = NULL;
    }
  
#endif
  return return_config;
}


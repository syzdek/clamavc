/*
 *  Clam Antivirus Client Library
 *  Copyright (C) 2009 Alaska Communications Systems Group, Inc
 *
 *  @ACS_LICENSE_HEADER_START@
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  @ACS_LICENSE_HEADER_END@
 */
/**
 *  @file lib/mod_clamavc.c ClamAVC proftpd module
 */
#define _CLAMAVC_LIB_MOD_CLAMAVC_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////

#include "contrib/clamavc.h"
#include "contrib/libclamavc.c"

#include "conf.h"
#include "privs.h"


//////////////
//          //
//  Macros  //
//          //
//////////////

#define MOD_CLAMAVC_VERSION	"mod_clamavc/0.1"

#if PROFTPD_VERSION_NUMBER < 0x0001021001
# error "ProFTPD 1.2.10rc1 or later required"
#endif


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////

static int clamavc_sess_init      PARAMS((void));
MODRET clamavc_pos_cmd            PARAMS((cmd_rec * cmd));
MODRET clamavc_set_enable         PARAMS((cmd_rec * cmd));
MODRET clamavc_set_host           PARAMS((cmd_rec * cmd));
MODRET clamavc_set_port           PARAMS((cmd_rec * cmd));
MODRET clamavc_set_streammaxlen   PARAMS((cmd_rec * cmd));
MODRET clamavc_set_maxfilesize    PARAMS((cmd_rec * cmd));


////////////////////////
//                    //
//  Global Variables  //
//                    //
////////////////////////

static int clamavc_engine = 0;

static conftable clamavc_conftab[] =
{
   { "Clamavc",               clamavc_set_enable,           NULL },
   { "ClamavcHost",           clamavc_set_host,             NULL },
   { "ClamavcPort",           clamavc_set_port,             NULL },
   { "ClamavcStreamMaxLen",   clamavc_set_streammaxlen,     NULL },
   { "ClamavcMaxFileSize",    clamavc_set_maxfilesize,      NULL},
   { NULL }
};


static cmdtable clamavc_cmdtab[] =
{
//  cmd_type   command  group    handler
  { POST_CMD,  C_STOR,  G_NONE,  clamavc_pos_cmd,   TRUE, FALSE },
  { POST_CMD,  C_STOU,  G_NONE,  clamavc_pos_cmd,   TRUE, FALSE },
  { POST_CMD,  C_APPE,  G_NONE,  clamavc_pos_cmd,   TRUE, FALSE },
  { 0, NULL }
};


module clamavc_module =
{
  NULL,              // pointer to the next module (define as NULL)
  NULL,              // pointer to the previous module (define as NULL)
  0x20,              // API version _not_ module version
  "clamavc",         // module name
  clamavc_conftab,   // Configuration directive table
  clamavc_cmdtab,    // Command table
  NULL,              // Authentication handler table
  NULL,              // Module initialization
  clamavc_sess_init  // Session initialization
};


////////////////////////
//                    //
//  Module Functions  //
//                    //
////////////////////////

MODRET clamavc_pos_cmd(cmd_rec * cmd)
{
   //int       maxfilesize;
   CLAMAVC    * clamp;
   config_rec * c;
   modret_t   * res;

   //maxfilesize = 0;

   if (!(clamavc_engine))
      return(DECLINED(cmd));

   if (!(clamp = clamavc_initialize()))
   {
      pr_log_pri(PR_LOG_ERR, MOD_CLAMAVC_VERSION ": error: Cannot allocate memory.");
      return(DECLINED(cmd));
   };

   if ((c = find_config(main_server->conf, CONF_PARAM, "ClamavcHost", FALSE)))
      clamavc_set_opt(clamp, CLAMAVC_OHOST, (char *) c->argv[0]);
   if ((c = find_config(main_server->conf, CONF_PARAM, "ClamavcPort", FALSE)))
      clamavc_set_opt(clamp, CLAMAVC_OPORT, (int *) c->argv[0]);
   if ((c = find_config(main_server->conf, CONF_PARAM, "ClamavcStreamMaxLen", FALSE)))
      clamavc_set_opt(clamp, CLAMAVC_OSTREAMMAXLEN, (int *) c->argv[0]);  
   //if ((c = find_config(main_server->conf, CONF_PARAM, "ClamavcMaxFileSize", FALSE)))
   //   maxfilesize = *((int *)c->argv[0]);

   if (clamavc_instream_file(clamp, cmd->argv[1]) == 1)
   {
      clamavc_close(clamp);
      unlink(cmd->argv[1]);
      pr_log_pri(PR_LOG_ERR, MOD_CLAMAVC_VERSION ": detected virus in %s", cmd->argv[1]);
      return(ERROR_MSG(cmd, R_451, "virus detected"));
   };

   clamavc_close(clamp);

   return(res);
}


static int clamavc_sess_init(void)
{
   config_rec * c;
   if (!(c = find_config(main_server->conf, CONF_PARAM, "Clamavc", FALSE)))
      return(0);
   if (*((unsigned int *) c->argv[0]) == TRUE)
      clamavc_engine = 1;
   return(0);
}


MODRET clamavc_set_enable(cmd_rec * cmd)
{
   int          bool;
   config_rec * c;

   CHECK_ARGS(cmd, 1);
	CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_ANON|CONF_DIR);

   if ((bool = get_boolean(cmd, 1)) == -1)
      CONF_ERROR(cmd, "expected Boolean parameter");

   c = add_config_param(cmd->argv[0], 1, NULL);
   c->argv[0] = pcalloc(c->pool, sizeof(unsigned int));
   *((unsigned int *) c->argv[0]) = bool;
   c->flags |= CF_MERGEDOWN;

   return(PR_HANDLED(cmd));
}


MODRET clamavc_set_host(cmd_rec * cmd)
{
   config_rec * c;

	CHECK_ARGS(cmd, 1);
	CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_DIR);

	c = add_config_param_str(cmd->argv[0], 1, (void *) cmd->argv[1]);
	c->flags |= CF_MERGEDOWN;

   return(PR_HANDLED(cmd));
}


MODRET clamavc_set_port(cmd_rec * cmd)
{
   config_rec * c;

	CHECK_ARGS(cmd, 1);
	CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_DIR);

	c = add_config_param_str(cmd->argv[0], 1, (void *) cmd->argv[1]);
	c->flags |= CF_MERGEDOWN;

   c = add_config_param(cmd->argv[0], 1, NULL);
   c->argv[0] = pcalloc(c->pool, sizeof(int));
   *((int *)c->argv[0]) = strtol(cmd->argv[1], NULL, 0);
   c->flags |= CF_MERGEDOWN;

   return(PR_HANDLED(cmd));
}


MODRET clamavc_set_streammaxlen(cmd_rec * cmd)
{
   config_rec * c;

	CHECK_ARGS(cmd, 1);
	CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_DIR);

	c = add_config_param_str(cmd->argv[0], 1, (void *) cmd->argv[1]);
	c->flags |= CF_MERGEDOWN;

   c = add_config_param(cmd->argv[0], 1, NULL);
   c->argv[0] = pcalloc(c->pool, sizeof(int));
   *((int *)c->argv[0]) = strtol(cmd->argv[1], NULL, 0);
   c->flags |= CF_MERGEDOWN;

   return(PR_HANDLED(cmd));
}


MODRET clamavc_set_maxfilesize(cmd_rec * cmd)
{
   config_rec * c;

	CHECK_ARGS(cmd, 1);
	CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_DIR);

	c = add_config_param_str(cmd->argv[0], 1, (void *) cmd->argv[1]);
	c->flags |= CF_MERGEDOWN;

   c = add_config_param(cmd->argv[0], 1, NULL);
   c->argv[0] = pcalloc(c->pool, sizeof(int));
   *((int *)c->argv[0]) = strtol(cmd->argv[1], NULL, 0);
   c->flags |= CF_MERGEDOWN;

   return(PR_HANDLED(cmd));
}


#pragma once
#include <code_test.h>

/******** Command definitions ********/
/**** General system commands ****/
#define CMD_SYS_FRATE             "SYS_FRATE"


typedef struct
{
	/* Settings */
	int                                  video_enabled;
	enum vfc_frame_rate                  sys_frm_rate;

} vfc_sys_if_cfg;

/* Enums */
enum vfc_frame_rate {
	VFC_FR_23_98 = 0,
	VFC_FR_24,
	VFC_FR_25,
	VFC_FR_29_97,
	VFC_FR_30,
	VFC_FR_50,
	VFC_FR_59_94,
	VFC_FR_60,
	VFC_FR_UNKNOWN,
};

/* Local types */
typedef int (*command_parser)(vfc_sys_if_cfg* cfg, char* cmd,
	unsigned int param_count);

/* Local struct definitions */
typedef struct
{
	const char*	   parse_string;
	int            parse_params;
	command_parser parse_func;
} vfc_cmd_parse_list;

typedef struct
{
	const char* name;
	int         val;
} str_enum_xlate_entry;

static const str_enum_xlate_entry f_rate_xlate_table[] =
{
	{ "23.98",  VFC_FR_23_98   },
	{ "24",     VFC_FR_24      },
	{ "25",     VFC_FR_25      },
	{ "29.97",  VFC_FR_29_97   },
	{ "30",     VFC_FR_30      },
	{ "50",     VFC_FR_50      },
	{ "59.94",  VFC_FR_59_94   },
	{ "60",     VFC_FR_60      },
	{ "?",      VFC_FR_UNKNOWN },
	{ nullptr,  VFC_FR_UNKNOWN },
};


int str_to_enum(const str_enum_xlate_entry* tbl_ptr, char* str, int* val);
const char* enum_to_str(const str_enum_xlate_entry* tbl_ptr, int val);
int split_args(char* in_str, char** out_strs, size_t max_args);
int get_buffer_command(char* in_buffer, char* out_cmd, size_t max_len);
int process_input_cmd(vfc_sys_if_cfg* cfg, char* parameter, char* cmd_string, int cmd_length);

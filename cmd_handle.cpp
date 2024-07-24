#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <cmd_handle.h>
#include <code_test.h>

int str_to_frame_rate(char* str, enum vfc_frame_rate* rate_freq)
{
	return str_to_enum(f_rate_xlate_table, str, (int*)rate_freq);
}

const char* frame_rate_to_str(enum vfc_frame_rate rate_freq)
{
	const char* str;
	str = enum_to_str(f_rate_xlate_table, (int)rate_freq);
	if (str == NULL)
		return "invalid";
	return str;
}

int vfc_parse_set_sys_frate(vfc_sys_if_cfg* cfg, char* cmd,
	unsigned int param_count)
{
	enum vfc_frame_rate new_rate;
	char* params[3];

	if (split_args(cmd, params, 3) != 2)
		return -1;
	if (str_to_frame_rate(params[1], &new_rate))
		return -2;
	if (new_rate != cfg->sys_frm_rate)
	{
		cfg->sys_frm_rate = new_rate;
	}

	return 0;
}

int vfc_parse_get_sys_frate(vfc_sys_if_cfg* cfg, char* cmd,
	unsigned int param_count)
{
	printf("frame rate: %s\n", frame_rate_to_str(cfg->sys_frm_rate));

	return 0;
}

/* In the format of 
*  { command string definition (as found in .h), number of arguments after definition, corresponding function }
*/
const vfc_cmd_parse_list set_cmd_list[] =
{
	/***** System commands *****/
	{ CMD_SYS_FRATE,          1, vfc_parse_set_sys_frate          },

	/***** Closing entry *****/
	{ NULL,                   0, NULL                             },
};

const vfc_cmd_parse_list get_cmd_list[] =
{
	/***** System commands *****/
	{ CMD_SYS_FRATE,          0, vfc_parse_get_sys_frate          },

	/***** Closing entry *****/
	{ NULL,                   0, NULL                             },
};




/************************************  Start of some boiler plate code
* 
*  you can read these functions but you're not expected to have to
*  edit anything in here so can be ignored
*/

#define UI_CMD_GET "GET "
#define UI_CMD_SET "SET "

/* Split an input string into constituate substrings, replacing all space-type
 * characters with NULL characters in the input string and setting pointers to
 * the start of the substrings. */
int split_args(char* in_str, char** out_strs, size_t max_args)
{
	int   argc = 0;
	char* in_ptr = in_str;

	while ((isprint(*in_ptr) || isspace(*in_ptr)) && (argc < max_args))
	{
		/* Replace all space characters with null characters */
		while (isspace(*in_ptr))
		{
			*in_ptr = '\0';
			in_ptr++;
		}
		if (!isprint(*in_ptr))
			return argc;
		out_strs[argc] = in_ptr;
		argc++;
		/* Where is the following space character? */
		while (!isspace(*in_ptr) && isprint(*in_ptr))
			in_ptr++;
	}

	return argc;
}

int strcasecmp(const char* s1, const char* s2)
{
	int c1, c2;

	do {
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
	} while (c1 == c2 && c1 != 0);

	return c1 - c2;
}

int str_to_enum(const str_enum_xlate_entry* tbl_ptr, char* str, int* val)
{
	if (tbl_ptr == NULL)
		return -1;
	while ((tbl_ptr->name != NULL) && (strcasecmp(tbl_ptr->name, str)))
		tbl_ptr++;
	if (tbl_ptr->name == NULL)
		return -2;
	if (val != NULL)
		*val = tbl_ptr->val;

	return 0;
}

const char* enum_to_str(const str_enum_xlate_entry* tbl_ptr, int val)
{
	if (tbl_ptr == NULL)
		return NULL;
	while ((tbl_ptr->name != NULL) && (tbl_ptr->val != val))
		tbl_ptr++;
	return tbl_ptr->name;
}

/* Counts the number of words in the input string.
 * Words are separated by blank characters. */
unsigned int count_parameters(char* input_cmd)
{
	unsigned int param_cnt = 0;
	int c_isblank;

	if (input_cmd == NULL)
		return 0;

	c_isblank = isblank(*input_cmd);
	if (!c_isblank)
		param_cnt++;


	int isWithinQuote = 0;
	while (*input_cmd != '\n' && *input_cmd != '\0')
	{
		if (*input_cmd == '\'' && !isWithinQuote)
			isWithinQuote = 1;
		else if (*input_cmd == '\'' && isWithinQuote)
			isWithinQuote = 0;
		if (!isblank(*input_cmd))
		{
			if (c_isblank)
			{
				c_isblank = 0;
				param_cnt++;
			}
		}
		else
		{
			if (!isWithinQuote)
				c_isblank = 1;
		}
		input_cmd++;
	}

	return param_cnt;
}

/* Take a 'GET' or 'SET' command and find the corresponding function to the
*  input string from the command line, executing it.
*/
static int vfc_parse_command(const vfc_cmd_parse_list* cmd_list,
	vfc_sys_if_cfg* cfg, char* parameter,
	char* cmd_string)
{
	const vfc_cmd_parse_list* list_ptr = cmd_list;

	while (list_ptr->parse_string != NULL)
	{
		if (!strncmp(parameter, list_ptr->parse_string,
			strlen(parameter)))
		{
			unsigned int param_count =
				count_parameters(cmd_string) - 1;
			if (list_ptr->parse_params == -1 &&
				list_ptr->parse_func != NULL)
				return list_ptr->parse_func(cfg, cmd_string,
					param_count);
			if (param_count == list_ptr->parse_params)
			{
				if (list_ptr->parse_func != NULL)
					return list_ptr->parse_func(cfg,
						cmd_string,
						param_count);
				/* Recognized but ignored */
				return -1;
			}
		}
		list_ptr++;
	}
	return -1;
}

/* Put the second word in the input buffer into the out_cmd buffer which has a
 * size of max_len bytes */
int get_buffer_command(char* in_buffer, char* out_cmd, size_t max_len)
{
	char* buf_ptr = in_buffer;
	char* cmd_ptr;
	size_t cmd_len = 0;

	memset(out_cmd, 0, max_len);
	/* Look for first blank space in input buffer */
	while (!isblank(*buf_ptr) && *buf_ptr != '\0')
		buf_ptr++;
	if (*buf_ptr == '\0')
		return -1;
	/* Now look for first non-blank character */
	while (isblank(*buf_ptr) && *buf_ptr != '\0')
		buf_ptr++;
	if (*buf_ptr == '\0')
		return -2;
	cmd_ptr = buf_ptr;
	/* Look for next blank space (or newline) in input buffer */
	while (!isspace(*buf_ptr) && *buf_ptr != '\0')
		buf_ptr++;
	/* How many bytes in the command? */
	cmd_len = buf_ptr - cmd_ptr;
	/* Clip max length of output */
	if (cmd_len > (max_len - 1))
		cmd_len = max_len - 1;
	/* Copy cmd_len bytes to the output buffer */
	strncpy(out_cmd, cmd_ptr, cmd_len);
	/* Add terminating NUL character */
	*(out_cmd + cmd_len + 1) = '\0';
	/* Done! */
	return 0;
}

/* parameter = word that comes after "GET" or "SET" in cmd_string.
* cmd_string =
* "(GET|SET) <parameter> [<param value 1>]..[<param value n>]\n" 
*/
int process_input_cmd(vfc_sys_if_cfg* cfg, char* parameter, char* cmd_string,
	int cmd_length)
{
	if (cfg == NULL)
	{
		return -1;
	}

	if (!strncmp(cmd_string, UI_CMD_GET, strlen(UI_CMD_GET)))
	{
		cmd_string += strlen(UI_CMD_GET);
		return vfc_parse_command(get_cmd_list, cfg, parameter,
			cmd_string);
	}
	if (!strncmp(cmd_string, UI_CMD_SET, strlen(UI_CMD_SET)))
	{
		cmd_string += strlen(UI_CMD_SET);
		return vfc_parse_command(set_cmd_list, cfg, parameter,
			cmd_string);
	}

	/* Command was not recognized */
	return -1;
}

/**************************************** End of boiler plate code
*/

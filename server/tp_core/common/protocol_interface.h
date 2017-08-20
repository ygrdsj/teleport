#ifndef __TP_PROTOCOL_INTERFACE_H__
#define __TP_PROTOCOL_INTERFACE_H__

#include "ts_const.h"
#include <ex.h>

#ifdef EX_OS_WIN32
#	ifdef TPP_EXPORTS
#		define TPP_API __declspec(dllexport)
#	else
#		define TPP_API __declspec(dllimport)
#	endif
#else
#	define TPP_API
#endif

typedef struct TPP_CONNECT_INFO
{
	char* sid;
	char* user_name;		// 申请本次连接的用户名
	char* real_remote_host_ip;	// 真正的远程主机IP（如果是直接连接模式，则与remote_host_ip相同）
	char* remote_host_ip;	// 要连接的远程主机的IP（如果是端口映射模式，则为路由主机的IP）
	char* account_name;		// 远程主机的账号
	char* account_secret;	// 远程主机账号的密码（或者私钥）
	char* user_param;
	int remote_host_port;	// 要连接的远程主机的端口（如果是端口映射模式，则为路由主机的端口）
	int protocol_type;
	int protocol_sub_type;
	//int auth_id;
	int auth_type;
	int sys_type;
	int ref_count;			// 这个连接信息的引用计数，如果创建的连接信息从来未被使用，则超过60秒后自动销毁
	ex_u64 ticket_start;	// 此连接信息的创建时间（用于超时未使用就销毁的功能）
}TPP_CONNECT_INFO;

typedef TPP_CONNECT_INFO* (*TPP_GET_CONNNECT_INFO_FUNC)(const char* sid);
typedef void(*TPP_FREE_CONNECT_INFO_FUNC)(TPP_CONNECT_INFO* info);
typedef bool(*TPP_SESSION_BEGIN_FUNC)(const TPP_CONNECT_INFO* info, int* db_id);
typedef bool(*TPP_SESSION_END_FUNC)(const char* sid, int db_id, int ret);


typedef struct TPP_INIT_ARGS
{
	ExLogger* logger;
	ex_wstr exec_path;
	ex_wstr etc_path;
	ex_wstr replay_path;
	ExIniFile* cfg;

	TPP_GET_CONNNECT_INFO_FUNC func_get_connect_info;
	TPP_FREE_CONNECT_INFO_FUNC func_free_connect_info;
	TPP_SESSION_BEGIN_FUNC func_session_begin;
	TPP_SESSION_END_FUNC func_session_end;
}TPP_INIT_ARGS;



#ifdef __cplusplus
extern "C"
{
#endif

	TPP_API ex_rv tpp_init(TPP_INIT_ARGS* init_args);
	TPP_API ex_rv tpp_start(void);
	TPP_API ex_rv tpp_stop(void);

#ifdef __cplusplus
}
#endif

typedef ex_rv (*TPP_INIT_FUNC)(TPP_INIT_ARGS* init_args);
typedef ex_rv (*TPP_START_FUNC)(void);
typedef ex_rv (*TPP_STOP_FUNC)(void);

#endif // __TP_PROTOCOL_INTERFACE_H__

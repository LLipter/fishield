#ifndef FISHIELD_H
#define FISHIELD_H

#include "fs_config.h"
#include "fs_client.h"
#include "fs_server.h"
#include "fs_task.h"
#include "fs_scheduler.h"

/***
 * DESC:    Try to start client program.
 *          This function should be called before any other function.
 *          Otherwise the result is undefined.
 * PARAM:   `addr` is the address of server.
 *          Both domain like "localhost" and digit representation like "127.0.0.1" would be ok.
 *          `port` is the port of server.
 * RETURN:  0 upon successful completion.
 *          FS_E_ILLEGAL_VALUE if `addr` is illegal or unreachable
 */
int fs_client_startup(const std::string& addr,
                      const short port);

/***
 * DESC:    Login to the system.
 *          This function should be called when client received a ILLEGALTOKEN response from server.
 * PARAM:   `cb_success` will be called when login successfully.
 *          Otherwise `cb_failed` will be called with a ResponseType as parameter.
 *          boost::bind() can be helpful when constructing proper function object.
 * RETURN:  return nothing
 */
void fs_login(const std::string& username,
              const std::string& password,
              fs_fp_void cb_success,
              fs_fp_error cb_failed);

void fs_filelist(const std::string& dirpath,
                 fs_fp_filelist cb_success,
                 fs_fp_error cb_failed);

void fs_mkdir(const std::string& path,
              fs_fp_void cb_success,
              fs_fp_error cb_failed);

void fs_remove(const std::string& path,
               fs_fp_void cb_success,
               fs_fp_error cb_failed);

void fs_rename(const std::string& oldpath,
               const std::string& newpath,
               fs_fp_void cb_success,
               fs_fp_error cb_failed);

void fs_register_task_callback(fs_fp_intdouble cb_progress,
                               fs_fp_int cb_success,
                               fs_fp_interror cb_failed);

void fs_upload(int client_id,
               const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename);

void fs_download(int client_id,
                 const std::string& localbasepath,
                 const std::string& remotebasepath,
                 const std::string& filename);

void fs_cancel(int client_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed);




void fs_server_startup(const short port);


#endif

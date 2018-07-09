#ifndef FISHIELD_H
#define FISHIELD_H

#include "fs_config.h"
#include "fs_client.h"
#include "fs_server.h"
#include "fs_task.h"
#include "fs_scheduler.h"

/***
 * NOTE:    Most of function provided by this project is asynchronous.
 *          A number of function object has been defined as callback.
 *          boost::bind() can be helpful when constructing proper function object.
 */


/***
 * DESC:    Try to start a client program.
 *          This function should be called before any other client function.
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
 * RETURN:  return nothing
 */
void fs_login(const std::string& username,
              const std::string& password,
              fs_fp_void cb_success,
              fs_fp_error cb_failed);

/***
 * DESC:    Retrieve a `FileList` Object from server.
 *
 * PARAM:   `dirpath` should be a full-virtual path represent a directory in server.
 *          `cb_success` will be called upon successful completion with a parameter of `FileList`.
 *          Otherwise `cb_failed` will be called with a ResponseType as parameter.
 * RETURN:  return nothing
 */
void fs_filelist(const std::string& dirpath,
                 fs_fp_filelist cb_success,
                 fs_fp_error cb_failed);

/***
 * DESC:    create a empty directory in server.
 *
 * PARAM:   `path` should be a full-virtual path represent a directory in server.
 *          `cb_success` will be called upon successful completion.
 *          Otherwise `cb_failed` will be called with a ResponseType as parameter.
 * RETURN:  return nothing
 */
void fs_mkdir(const std::string& path,
              fs_fp_void cb_success,
              fs_fp_error cb_failed);

/***
 * DESC:    remove a file in server.
 *
 * PARAM:   `path` should be a full-virtual path represent a file in server.
 *          `cb_success` will be called upon successful completion.
 *          Otherwise `cb_failed` will be called with a ResponseType as parameter.
 * RETURN:  return nothing
 */
void fs_remove(const std::string& path,
               fs_fp_void cb_success,
               fs_fp_error cb_failed);
/***
 * DESC:    rename a file in server.
 *
 * PARAM:   A file represented by `oldpath` by be renamed to a file represented by `newpath`.
 *          `cb_success` will be called upon successful completion.
 *          Otherwise `cb_failed` will be called with a ResponseType as parameter.
 * RETURN:  return nothing
 */
void fs_rename(const std::string& oldpath,
               const std::string& newpath,
               fs_fp_void cb_success,
               fs_fp_error cb_failed);

/***
 * DESC:    register callback function for upload/download task.
 *          This function should be called before fs_upload()/fs_download().
 *          Otherwise, default callback function will be invoked.
 *
 * PARAM:   `cb_progress` will be invoked whenever a packet is sent/received.
 * RETURN:  return nothing
 */
void fs_register_task_callback(fs_fp_intdouble cb_progress,
                               fs_fp_int cb_success,
                               fs_fp_interror cb_failed);

/***
 * DESC:    initiate a new upload task.
 *
 * PARAM:   Both `localbasepath` and `remotebasepath` shouldn't ends with '/'(path separator).
 * RETURN:  return nothing
 */
void fs_upload(int client_id,
               const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename);

/***
 * DESC:    initiate a new download task.
 *
 * PARAM:   Both `localbasepath` and `remotebasepath` shouldn't ends with '/'(path separator).
 * RETURN:  return nothing
 */
void fs_download(int client_id,
                 const std::string& localbasepath,
                 const std::string& remotebasepath,
                 const std::string& filename);

void fs_cancel(int client_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed);



/***
 * DESC:    Try to start a server program.
 * PARAM:   `port` is the port of server.
 * RETURN:  This function will never return.
 */
void fs_server_startup(const short port);


#endif

#ifndef FISHIELD_H
#define FISHIELD_H

#include "fs_config.h"
#include "fs_client.h"
#include "fs_server.h"
#include "fs_scheduler.h"
#include "fs_error.h"


int fs_client_startup(const std::string& addr,
                      const short port);

void fs_login(const std::string& username,
              const std::string& password,
              fs_fp_void cb_success,
              fs_fp_error cb_failed);

void fs_get_filelist(const std::string& dirpath,
                     fs_fp_filelist cb_success,
                     fs_fp_error cb_failed);

void fs_mkdir(const std::string& basepath,
              const std::string& dirname,
              fs_fp_void cb_success,
              fs_fp_error cb_failed);

void fs_upload(const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename,
               fs_fp_intdouble cb_progress,
               fs_fp_int cb_success,
               fs_fp_error cb_failed);

void fs_download(const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename,
               fs_fp_intdouble cb_progress,
               fs_fp_int cb_success,
               fs_fp_error cb_failed);


void fs_server_startup(const short port);


#endif

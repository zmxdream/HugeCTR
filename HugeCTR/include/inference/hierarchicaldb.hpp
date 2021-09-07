/*
 * Copyright (c) 2021, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <inference/redis_cluster.h>
#include <string.h>  //strcmpcase

#include <cstdlib>
#include <inference/database.hpp>
#include <inference/inference_utils.hpp>
#include <inference/redis.hpp>
#include <inference/rocksdb.hpp>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace HugeCTR {

template <typename TypeHashKey>
class hierarchical_db : public DataBase<TypeHashKey> {
 public:
  hierarchical_db();
  hierarchical_db(redis<TypeHashKey>* redis_context, rocks_db<TypeHashKey>* rocksdb_context);
  hierarchical_db(parameter_server_config ps_config);
  virtual ~hierarchical_db();

  void init(const std::string& KDBPath, const std::string& ip, int port, const std::string& pwd,
            const float cache_size_percentage = 0.5);
  void look_up(const TypeHashKey* embeddingcolumns_ptr, size_t num_samples,
               float* h_embeddingoutputvector, const std::string& model_name,
               size_t embedding_table_id);

  void connect();
  void disconnect();

  void load_data(const std::string& model_config_path);
  bool mset(const std::vector<TypeHashKey>& keys, std::vector<float>& values,
            const std::string modelname, const std::string tablename, size_t embedding_size);
  bool mget(const TypeHashKey* keys, std::vector<float>& values, size_t length,
            const std::string modelname, const std::string tablename, size_t embedding_size);
  bool cmget(const TypeHashKey* keys, std::vector<float>& values, size_t length,
             const std::string modelname, const std::string tablename, size_t embedding_size);

 private:
  void freeReply();
  bool isError(rocksdb::Status s);

 private:
  // The Data Base name
  std::string data_base_name = "hierarchical_db";
  // The parameter server configuration
  parameter_server_config ps_config_;
  redis<TypeHashKey>* redis_context;
  rocks_db<TypeHashKey>* rocksdb_context;
  std::string _ip;
  int32_t _port;
  std::string _pwd;
  std::string _kdbpath;
  bool _connected;
  r3c::CRedisClient* rc_client_t;
  float cache_size_percentage_ = 0.5;
};

}  // namespace HugeCTR
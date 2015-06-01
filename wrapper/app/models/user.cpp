#include "user.h"

UserModel::UserModel() {
  return;
}

UserModel::~UserModel() {
  return;
}

static int UserModel::save(UserModel model) {
  // Check if user table exists
  try {
  } catch(pqxx::broken_connection &e) {
    std::cout << "[-] Broken connection => " << e.base().what() << std::endl;
    exit(1);
  }
  return 0;
}

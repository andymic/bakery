/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-18T21:43:50-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Anycast.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-18T21:51:28-04:00
 */
#include<string>

struct JoinProxy{
  int id;
  std::string location;
}

struct IngressProxy{
  int id;
  std::string location;
}

struct RendezvousProxy{
  int id;
  std::string location;
}

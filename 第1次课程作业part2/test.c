#include<sys/syscall.h>
#include<unistd.h>
#include<linux/psta.h>

//int syscall(int number,…);
//sys_psta
struct pinfo info;

int main(){
  int ret = syscall(320,&info);
  return 0;
}

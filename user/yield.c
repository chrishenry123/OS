#include <sys_req.h>
#include <yield.h>

void yield(){
	sys_req(IDLE);
}

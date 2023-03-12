#include "headfile.h"

int main(void)
{
	DisableInterrupts;
    get_clk();//获取时钟频率  务必保留
	
		
	//总中断最后开启
    EnableInterrupts;
    while (1)
    {
	
    }
}


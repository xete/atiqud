#include "ext_int.h"

void ext_int_on_p1_2(void)
{
	/* clear interrupt flags */
	P1IFG = 0x00;
	PICTL |= 0x02;
	/* port 1 interrupt mask P2 */
	P1IEN = 0x04;
	IEN2 |= 0x10;
	/* EA */
	IEN0 |= 0x80;
}

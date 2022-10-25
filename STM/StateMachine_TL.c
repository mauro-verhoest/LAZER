#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "Interrupts.h"


struct state;
typedef void states(struct state *);

struct state
{
	states *next;

	int Ammo;
	int Healt;
};

states shoot_state, hit_state, hitby_state, reload_state;

void shoot_state(struct state * state)
{
	printf("PIEUW \n");
}

void hit_state(struct state * state)			// Software interrupt
{
	printf("YES \n");
}

void hitby_state(struct state * state)			// Software interrupt
{
	printf("GERAAKT! \n");
}

void reload_state(struct state * state)
{
	printf("RELOAD! \n");
}

int main(void)
{
	init_interrupts(*shoot_state, *reload_state);

	return 0;
}

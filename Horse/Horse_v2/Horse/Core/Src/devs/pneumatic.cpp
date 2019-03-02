#include "pneumatic.h"
#include "robo_states.h"

extern State gHorse_State;


enum class Pneu_Index
{
        P_1,
        P_2,
        P_3,
        P_4,
        P_NONE
};

class Pneumatic
{
      private:
        GPIO_TypeDef *pneu_port;
        uint16_t pneu_pin;

      public:
        Pneumatic() { }

        void init(GPIO_TypeDef *port, uint16_t pin)
        {
                pneu_port = port;
                pneu_pin = pin;
        }

        void set(void);
        void reset(void);
};

/*to actuate the pneumatic*/
void Pneumatic::set()
{
        HAL_GPIO_WritePin(pneu_port, pneu_pin, GPIO_PIN_SET);
}

/*to deactuate the pneumatic*/
void Pneumatic::reset()
{
        HAL_GPIO_WritePin(pneu_port, pneu_pin, GPIO_PIN_RESET);
}

Pneumatic gPneu[4];
/*initialize pneumatics*/
void pneu_Init()
{
        gPneu[(int)(Pneu_Index::P_1)].init(GPIOC, GPIO_PIN_1);
        gPneu[(int)(Pneu_Index::P_2)].init(GPIOE, GPIO_PIN_8);
        gPneu[(int)(Pneu_Index::P_3)].init(GPIOB, GPIO_PIN_2);
        gPneu[(int)(Pneu_Index::P_4)].init(GPIOC, GPIO_PIN_3);
}

static void set_Index(Pneumatic (&arr)[4], Pneu_Index index)
{
        if (index != Pneu_Index::P_NONE)
        {
                arr[(int)index].set();
        }
        for (uint8_t i = 0; i < (int)(Pneu_Index::P_NONE); ++i)
        {
                if (i != (int)index)
                {
                        arr[i].reset();
                }
        }
}

/*read the state of the system*/
void pneu_Loop()
{
        switch (gHorse_State.get_ID())
        {
        	case State_ID::HOME:
        	{
        	        set_Index(gPneu, Pneu_Index::P_NONE);
        	}
        	break;
	
        	case State_ID::WS1:
        	{
        	        set_Index(gPneu, Pneu_Index::P_1); 
        	}
        	break;
	
        	case State_ID::WS2:
        	{
        	        set_Index(gPneu, Pneu_Index::P_2);
        	}
        	break;
	
        	case State_ID::WS3:
        	{
        	        set_Index(gPneu, Pneu_Index::P_3);
        	}
        	break;
	
        	case State_ID::WS4:
        	{
        	        set_Index(gPneu, Pneu_Index::P_4);
        	}
        	break;
        }
}
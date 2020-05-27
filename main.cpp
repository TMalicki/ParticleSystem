#include <iostream>
#include "ParticleSystem.h"

// turn on randomness walk particle - needed
// after all particles on the screen erased and new explosion wanna be made - error occured
// friction should be working only in y direction? Or should i choose in which direction that works (x and y, or just one of them)
// emiter sometimes is erasing while using erasingBorders when it should not happen
// also - emiter should be sigle particle at the time, it should have life spawn and it should have rateSpawn method
// void ParticleManage::applyFading() ----- do template of that?

int main()
{
    ParticleSystem particleSystem;
    particleSystem.Run();

    return 0;
}
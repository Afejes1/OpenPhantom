#include "api.h"
// FUNCTION: WMAIN 0x0045200e
int op_save_disk_free_at_least(unsigned int requested)
{
    unsigned int freeSpace = 0;
    unsigned int sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
    int success;
    success = op_disk_free_space(0, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);
    if (!success)
        return 1;
    freeSpace = numberOfFreeClusters * sectorsPerCluster * bytesPerSector;
    if (requested < freeSpace)
        return 1;
    return 0;
}

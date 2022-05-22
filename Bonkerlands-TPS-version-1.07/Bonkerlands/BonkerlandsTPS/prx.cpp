#include "include.h"

#pragma diag_suppress 77
SYS_MODULE_INFO(BonkerlandsTPS, 0, 1, 1);
SYS_MODULE_START(start_engine);
SYS_MODULE_STOP(stop_engine);



extern "C" {
    int start_engine() {
        sys_ppu_thread_t thread;
        sys_ppu_thread_create(&thread, [](uint64_t /*unused*/) {
            libpsutil::memory::nop(0x0054FDA0);//trophy init nop
            libpsutil::memory::nop(0x0054B710);//idk something i crash on randomly but seems to still work without

            if (!libpsutil::filesystem::directory_exists(BONKERLANDS_DIR)) {
                libpsutil::filesystem::create_directory(BONKERLANDS_DIR);
            }

            libpsutil::sleep(30000);
            bltps::ui::start();
            scheduler::start();

            sys_ppu_thread_exit(0);
            }, NULL, 0x7FC, 0x100, 0, "boot");

        return SYS_PRX_RESIDENT;
    }

    int stop_engine() {
        bltps::ui::unload_project = true;
        
        printf("quit\n");

        libpsutil::sleep(400);

        scheduler::stop();
        bltps::ui::stop();
        return SYS_PRX_RESIDENT;
    }
}

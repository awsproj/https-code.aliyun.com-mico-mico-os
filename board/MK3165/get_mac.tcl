proc get_mac { } {
    init
    reset halt
    
    load_image mico-os/board/MK3165/get_mac.elf
    set mac_loc 0x20009350
    mww $mac_loc 0x00000000

    reg pc 0x20009050
    reg sp 0x2000a154
    
    resume
    set mac_val 0x000000000000
    set loops  0
    while { ($mac_val == 0x000000000000) && ( $loops < 20 ) } {
        sleep 100
        mem2array mac_arr 8 $mac_loc 6
        set mac_val [expr $mac_arr(5)+($mac_arr(4)<<8)+($mac_arr(3)<<16)+($mac_arr(2)<<24)+($mac_arr(1)<<32)+($mac_arr(0)<<40)]
        incr loops
    }

    if { $mac_val == 0x000000000000 } {
        halt
        error "Read MAC address timeout"
        exit -1;
    }

    puts [format "%X" $mac_val]
}

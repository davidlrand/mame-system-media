/* structure for TIMES system call */
struct tms {
    int tms_utime;
    int tms_stime;
    int tms_cutime;
    int tms_cstime;
};

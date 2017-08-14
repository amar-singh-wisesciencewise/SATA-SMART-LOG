# SATA-SMART-LOG
This Program sends a read SMART Log Command to a SATA Drive. And interprets the data thus returned. 

THIS CODE HAS BEEN WRITTEN FOR LINUX.

Compiling: gcc file.c
Execution: sudo ./a.out /dev/sg<>

"identify.c" program  sends identify command to an SATA drive and interpret data thus returned.
"smart-log.c" program sends a READ SMART LOG Command to an SATA Drive and interprets it.

Link to my Blog (for more details and steps explanation):
https://wisesciencewise.wordpress.com/2017/03/19/c-program-to-read-smart-log-of-a-sata-drives/

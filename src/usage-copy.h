/*
  Copyright (C) 1999, 2005, 2008, 2009 Rocky Bernstein
*/

const char usage_help[] =
"USAGE:\n"
"  cd-paranoia [options] <span> [outfile]\n"
"\n"
"OPTIONS:\n"
"  -v --verbose                    : extra verbose operation\n"
"  -q --quiet                      : quiet operation\n"
"  -e --stderr-progress            : force output of progress information to\n"
"  -l --log-summary         <file> : save result summary to file\n"
"                                    stderr (for wrapper scripts)\n"
"  -V --version                    : print version info and quit\n"
"  -Q --query                      : autosense drive, query disc and quit\n"
"  -B --batch                      : 'batch' mode (saves each track to a\n"
"                                    seperate file.\n"
"  -s --search-for-drive           : do an exhaustive search for drive\n"
"  -h --help                       : print help\n"
"\n"
"  -p --output-raw                 : output raw 16 bit PCM in host byte \n"
"                                    order\n"
"  -r --output-raw-little-endian   : output raw 16 bit little-endian PCM\n"
"  -R --output-raw-big-endian      : output raw 16 bit big-endian PCM\n"
"  -w --output-wav                 : output as WAV file (default)\n"
"  -f --output-aiff                : output as AIFF file\n"
"  -a --output-aifc                : output as AIFF-C file\n"
"\n"
"  -c --force-cdrom-little-endian  : force treating drive as little endian\n"
"  -C --force-cdrom-big-endian     : force treating drive as big endian\n"
"  -n --force-default-sectors <n>  : force default number of sectors in read\n"
"                                    to n sectors\n"
"  -o --force-search-overlap  <n>  : force minimum overlap search during\n"
"                                    verification to n sectors\n"
"  -d --force-cdrom-device   <dev> : use specified device; disallow \n"
"                                    autosense\n"
"  -g --force-generic-device <dev> : really an alias for -d. Kept for \n"
"                                    compatibility.\n"
"  -S --force-read-speed <n>       : read from device at specified speed\n"
"  -t --toc-offset <n>             : Add <n> sectors to the values reported\n"
"                                    when addressing tracks. May be negative\n"
"  -T --toc-bias                   : Assume that the beginning offset of \n"
"                                    track 1 as reported in the TOC will be\n"
"                                    addressed as LBA 0.  Necessary for some\n"
"                                    Toshiba drives to get track boundaries\n"
"                                    correct\n"
"  -m --mmc-timeout <n>            : Set SCSI-MMC timeout to <n> seconds.\n"
"  -O --sample-offset <n>          : Add <n> samples to the offset when\n"
"                                    reading data.  May be negative.\n"
"  -z --never-skip[=n]             : never accept any less than perfect\n"
"                                    data reconstruction (don't allow 'V's)\n"
"                                    but if [n] is given, skip after [n]\n"
"                                    retries without progress.\n"
"  -Z --disable-paranoia           : disable all paranoia checking\n"
"  -Y --disable-extra-paranoia     : only do cdda2wav-style overlap checking\n"
"  -X --abort-on-skip              : abort on imperfect reads/skips\n"
"  -x --test-flags=mask            : simulate CD-reading errors of ilk-mask n\n"
"                                    mask & 0x10  - simulate underrun errors\n"
"\n"
"OUTPUT SMILIES:\n"
"  :-)   Normal operation, low/no jitter\n"
"  :-|   Normal operation, considerable jitter\n"
"  :-/   Read drift\n"
"  :-P   Unreported loss of streaming in atomic read operation\n"
"  8-|   Finding read problems at same point during reread; hard to correct\n"
"  :-0   SCSI/ATAPI transport error\n"
"  :-(   Scratch detected\n"
"  ;-(   Gave up trying to perform a correction\n"
"  8-X   Aborted (as per -X) due to a scratch/skip\n"
"  :^D   Finished extracting\n"
"\n"
"PROGRESS BAR SYMBOLS:\n"
"<space> No corrections needed\n"
"   -    Jitter correction required\n"
"   +    Unreported loss of streaming/other error in read\n"
"   !    Errors are getting through stage 1 but corrected in stage2\n"
"   e    SCSI/ATAPI transport error (corrected)\n"
"   V    Uncorrected error/skip\n"
"\n"
"SPAN ARGUMENT:\n"
"The span argument may be a simple track number or a offset/span\n"
"specification.  The syntax of an offset/span takes the rough form:\n"
"  \n"
"                       1[ww:xx:yy.zz]-2[aa:bb:cc.dd] \n"
"\n"
"Here, 1 and 2 are track numbers; the numbers in brackets provide a\n"
"finer grained offset within a particular track. [aa:bb:cc.dd] is in\n"
"hours/minutes/seconds/sectors format. Zero fields need not be\n"
"specified: [::20], [:20], [20], [20.], etc, would be interpreted as\n"
"twenty seconds, [10:] would be ten minutes, [.30] would be thirty\n"
"sectors (75 sectors per second).\n"
"\n"
"When only a single offset is supplied, it is interpreted as a starting\n"
"offset and ripping will continue to the end of he track.  If a single\n"
"offset is preceeded or followed by a hyphen, the implicit missing\n"
"offset is taken to be the start or end of the disc, respectively. Thus:\n"
"\n"
"    1:[20.35]    Specifies ripping from track 1, second 20, sector 35 to \n"
"                 the end of track 1.\n"
"\n"
"    1:[20.35]-   Specifies ripping from 1[20.35] to the end of the disc\n"
"\n"
"    -2           Specifies ripping from the beginning of the disc up to\n"
"                 (and including) track 2\n"
"\n"
"    -2:[30.35]   Specifies ripping from the beginning of the disc up to\n"
"                 2:[30.35]\n"
"\n"
"    2-4          Specifies ripping from the beginning of track two to the\n"
"                 end of track 4.\n"
"\n"
"Don't forget to protect square brackets and preceeding hyphens from\n"
"the shell...\n"
"\n"
"A few examples, protected from the shell:\n"
"  A) query only with exhaustive search for a drive and full reporting\n"
"     of autosense:\n"
"       cd-paranoia -vsQ\n"
"\n"
"  B) extract up to and including track 3, putting each track in a seperate\n"
"     file:\n"
"       cd-paranoia -B -- \"-3\"\n"
"\n"
"  C) extract from track 1, time 0:30.12 to 1:10.00:\n"
"       cd-paranoia \"[:30.12]-1[1:10]\"\n"
"\n"
"Submit bug reports to bug-libcdio@gnu.org\n"
"\n"
;
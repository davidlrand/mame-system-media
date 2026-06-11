while ($file = shift) {
   open (FI,"<$file") || die "can't open $file:$!";
   open (FO,">t") || die "Can't open temp file: $!";
   while (<FI>) {
       chop;
       s///;
       s///g;
       tr [\200-\377] [\000-\177];
       print FO "$_\n";
    }
    close(FI);
    close(FO);
    unlink($file);
    rename("t",$file) || die "Can't rename $file:$!";
}

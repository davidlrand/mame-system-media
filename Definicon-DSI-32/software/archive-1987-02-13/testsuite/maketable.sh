#! /bin/sh

VERSION="v0.7"
COPYRIGHT="(w) 2003-2014 Groepaz/Hitmen"

#########################################################################################
# util stuff
#########################################################################################


cattesttag()
{
    grep "\!\!$3\!\!" $1 | sed -e 's/\!\!'$3'\!\!//' >> $2
}

#########################################################################################

reporttop()
{
	echo "<!-- testsuite report -->" > $OUTFILE
	echo "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">" >> $OUTFILE
	echo "<HTML>" >> $OUTFILE
	echo "<HEAD>" >> $OUTFILE
	echo "<title>testsuite report</title>" >> $OUTFILE
	echo "<META http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">" >> $OUTFILE
	echo "</HEAD>" >> $OUTFILE

	echo "<BODY>" >> $OUTFILE
	echo "<table align=\"center\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\"><tr><td>" >> $OUTFILE
	echo "<table bgcolor=\"#c0c0c0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
	echo "<table bgcolor=\"#c0c0c0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
}

reportbottom()
{
	echo "</td></tr></table>" >> $OUTFILE
	echo "</td></tr></table>" >> $OUTFILE
	echo "</td></tr></table>" >> $OUTFILE
	echo "</BODY>">> $OUTFILE
	echo "</HTML>">> $OUTFILE
}


makecell()
{
	TESTPREFIX=$TARGETSTD-o$TARGETOPTS

#	echo PREF:$TESTPREFIX.$TESTNAME.status

	COLOR="#a0a0a0";
	STAT="ERR";

	if test -s ./temp/$TESTPREFIX.$TESTNAME.status; then
	STAT=`cat ./temp/$TESTPREFIX.$TESTNAME.status`
	fi;
	
	if test $STAT = "000"; then COLOR="#c0ffc0"; fi
	if test $STAT = "101"; then COLOR="#ffc0c0"; fi
	if test $STAT = "100"; then COLOR="#ff8080"; fi
	if test $STAT = "110"; then COLOR="#c06060"; fi
	if test $STAT = "ERR"; then COLOR="#c0c0c0"; fi
	
        echo "<td width=\"10%\" bgcolor=\"$COLOR\">" >> $OUTFILE
	if test $STAT != "ERR"; then
	echo "<a href=\"./html/$TESTPREFIX.$TESTNAME.diff.html"\"">"$TESTNAME"</a>" >> $OUTFILE
#	echo $TESTNAME >> $OUTFILE
#	echo $STAT >> $OUTFILE
	else
	echo "N/A" >> $OUTFILE
	fi;
	echo "</td>" >> $OUTFILE
}

makerow()
{
	echo "<tr width=\"100%\" ALIGN=\"left\" bgcolor=\"#a0a0a0\"><td>" >> $OUTFILE


        cattesttag $TESTBASE.c $OUTFILE DESCRIPTION
	
	echo "</td>" >> $OUTFILE
	
#        TARGETOPTS=""
#        TARGETSTD=cc65
#        makecell
#        TARGETSTD=c89
#        makecell
#        TARGETSTD=c99
#        makecell
#        TARGETOPTS="-Osir"
#        TARGETSTD=cc65
#        makecell
#        TARGETSTD=c89
#        makecell
#        TARGETSTD=c99
#        makecell

	TARGETOPTS="";TARGETSTD="cc65";makecell;
	TARGETOPTS="-O";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Os";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Oi";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Or";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Osi";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Osr";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Oir";TARGETSTD="cc65";makecell;
	TARGETOPTS="-Osir";TARGETSTD="cc65";makecell;
        
	echo "</tr>" >> $OUTFILE
}

#
# create report
#

export TESTPREFIX=$TARGETSTD-o$TARGETOPTS
export OUTFILE=report-overview.html

reporttop

echo "<table bgcolor=\"#a0a0a0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
echo "<table bgcolor=\"#a0a0a0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE

echo "<h1>automatic compiler testsuite "$VERSION"</h1>" >> $OUTFILE
echo "<small>"$COPYRIGHT"</small>" >> $OUTFILE

echo "</td></tr></table>" >> $OUTFILE
echo "<br></td></tr></table>" >> $OUTFILE

echo "<p><a href=\"./index.html\">Index</a>" >> $OUTFILE
echo " Overview" >> $OUTFILE

echo "<h2>overview:</h2>" >> $OUTFILE

echo "<table bgcolor=\"#c0c0c0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
echo "<table bgcolor=\"#c0c0c0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE

echo "<b>target:</b>" $TARGET >> $OUTFILE
echo "&nbsp&nbsp&nbsp<b>target compiler:</b>" $TARGETCC >> $OUTFILE
echo "("` cat ./temp/targetcc.version `")"  >> $OUTFILE
echo "&nbsp&nbsp&nbsp<b>reference compiler:</b>" $REFCC >> $OUTFILE

echo "<br><br><b>options:</b>&nbsp&nbsp&nbsp" >> $OUTFILE
grep -i "#define" ./arch/$TARGET/targetconf.h | grep -v "//" | grep -v "/\*" | sed -e 's/#define/\&nbsp\;\&nbsp\;/g' >> $OUTFILE

echo "<br><br><b>reference options:</b>&nbsp&nbsp&nbsp" >> $OUTFILE
grep -i "#define" ./arch/$REFCC/refconfig.h | grep -v "//" | grep -v "/\*" | sed -e 's/#define/\&nbsp\;\&nbsp\;/g' >> $OUTFILE

echo "</td></tr></table>" >> $OUTFILE
echo "<br></td></tr></table>" >> $OUTFILE

echo "<table bgcolor=\"#a0a0a0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
echo "<table bgcolor=\"#a0a0a0\" align=\"center\" width=\"99%\">" >> $OUTFILE

echo "<tr><td></td>" >> $OUTFILE

#TARGETOPTS=""
#echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
#echo "<td><a href=\"report-c89-o$TARGETOPTS.html\">c89 $TARGETOPTS</a></td>" >> $OUTFILE
#echo "<td><a href=\"report-c99-o$TARGETOPTS.html\">c99 $TARGETOPTS</a></td>" >> $OUTFILE
#TARGETOPTS="-Osir"
#echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
#echo "<td><a href=\"report-c89-o$TARGETOPTS.html\">c89 $TARGETOPTS</a></td>" >> $OUTFILE
#echo "<td><a href=\"report-c99-o$TARGETOPTS.html\">c99 $TARGETOPTS</a></td>" >> $OUTFILE

TARGETOPTS="";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-O";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Os";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Oi";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Or";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Osi";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Osr";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Oir";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE
TARGETOPTS="-Osir";echo "<td><a href=\"report-cc65-o$TARGETOPTS.html\">cc65 $TARGETOPTS</a></td>" >> $OUTFILE

echo "</tr>" >> $OUTFILE

for I in $TESTS ; do \
    TESTBASE=$I; \
    i=`echo "$I" | sed -e 's/.*\///'`; \
	echo -n "$i,"; \
	TESTNAME=$i; \
    	makerow; \
	done

echo "</table>" >> $OUTFILE
echo "<br></td></tr></table>" >> $OUTFILE

reportbottom


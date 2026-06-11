#! /bin/sh

VERSION="v0.7"
COPYRIGHT="(w) 2003-2014 Groepaz/Hitmen"

#########################################################################################
# util stuff
#########################################################################################

cat2html()
{

        echo "<tt>" >> $2
#			     -e 's/\t/        /g' \

        cat -s $1 | tr "\t" "        " | \
                sed \
                 -e 's/‘/\&acute;/g' \
                 -e 's/`/\&#96;/g' \
                 -e 's/‘/\&#8216;/g' \
                 -e 's/’/\&#8217;/g' \
                 -e 's/</\&lt;/g' \
                 -e 's/>/\&gt;/g' \
                 -e 's/ /\&nbsp;/g' \
                 -e 's/$/<br>/g' \
                 -e 's/\.\{1\}\(.\)\([(:]\)\([0-9]\+\)\([):]\)/.\1\2<a href=.\/'$TESTNAME'.\1.html\#line\3>\3<\/a>\4/' \
                 -e 's/\([(:]\)\([0-9]\+\)\([):]\)/\1<a href=.\/'$TESTNAME'.c.html\#line\2>\2<\/a>\3/' \
                 >> $2

        echo "</tt>" >> $2
}

#                 -e 's/\([(:]\)\([0-9]\+\)\([):]\)/\1<a href=.\/'$TESTNAME'.c.html\#line\2>\2<\/a>\3/' \

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

testshort()
{
	echo "<table bgcolor=\"$TESTCOLOR\" border=\"2\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\"><tr><td>" >> $OUTFILE
	echo "<table bgcolor=\"$TESTCOLOR\"  align=\"center\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"99%\">" >> $OUTFILE
	echo "<tr width=\"100%\" ALIGN=\"left\"><td>" >> $OUTFILE
	echo "<h2>" >> $OUTFILE
	echo "<a href=\"./html/$TESTPREFIX.$TESTNAME.diff.html"\"">"$TESTNAME"</a>" >> $OUTFILE
	echo "</h2>" >> $OUTFILE
	echo "</td><td align=\"right\">" >> $OUTFILE
#	if test -s ./files/$TESTNAME.description; then \
#		cat ./files/$TESTNAME.description >> $OUTFILE; \
#	fi;
#    grep "\!\!DESCRIPTION\!\!" $TESTBASE.c | sed -e 's/\!\!DESCRIPTION\!\!//' >> $OUTFILE; \

    cattesttag $TESTBASE.c $OUTFILE DESCRIPTION

    if test -n "$TESTTEXT" ; then \
	    echo "<br><tt><small>"$TESTTEXT"</small></tt><br>" >> $OUTFILE; \
    fi;
	echo "</td></tr></table>" >> $OUTFILE
	echo "</td></tr></table>" >> $OUTFILE
}

testlong()
{
	echo "<!-- testsuite report -->" > ./html/$TESTPREFIX.$TESTNAME.diff.html

	echo "<table bgcolor=\"$TESTCOLOR\" border=\"2\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\"><tr><td>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo "<table bgcolor=\"$TESTCOLOR\"  align=\"center\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"99%\">" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo "<tr width=\"100%\" ALIGN=\"left\"><td>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo "<h1>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo $TESTNAME >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo "</h1>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo "</td><td align=\"right\">" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
#	if test -s ./files/$TESTNAME.description; then \
#		cat ./files/$TESTNAME.description >> ./html/$TESTNAME.diff.html; \
#	fi;
    grep "\!\!DESCRIPTION\!\!" $TESTBASE.c | sed -e 's/\!\!DESCRIPTION\!\!//' >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
    if test -n "$TESTTEXT" ; then \
	    echo "<br><tt><small>"$TESTTEXT"</small></tt><br>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
    fi;
	echo "</td></tr></table>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo "</td></tr></table>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html

        echo "<p><a href=\"./../index.html\">Index</a>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
        echo " <a href=\"./../report-overview.html\">Overview</a>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
        echo " <a href=\"./../report-"$TESTPREFIX".html\">List</a>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html

	echo " C-Source: <a href=\""./$TESTNAME.c.html\"">"$TESTNAME.c"</a>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	echo " generated assembler code: <a href=\""./$TESTPREFIX.$TESTNAME.s.html\"">"$TESTNAME.s"</a>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html

	if test -e $TESTBASE.input; then \
		echo " Input File: <a href=\""./$TESTNAME.input\"">"$TESTNAME.input"</a>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html ; \
		cat $TESTBASE.input > ./html/$TESTNAME.input ; \
	fi

    set -- $C2HTML
    if which $1 > /dev/null; then \
    $C2HTML < $TESTBASE.c > ./html/$TESTNAME.c.html; \
	else \
	echo "" > ./html/$TESTNAME.c.html; \
    cat2html $TESTBASE.c ./html/$TESTNAME.c.html; \
	fi


#
# fixme: add a switch for other targets compilers asm-output
#

	if test -s $TESTBASE.s; then \
	if which $ASM2HTML > /dev/null; then \
		ca65html --colorize --crefs --cvttabs --linelabels --linenumbers $TESTBASE.s --htmldir ./html; \
		mv ./html/$TESTNAME.s.html ./html/$TESTPREFIX.$TESTNAME.s.html; \
        else \
            echo "" > ./html/$TESTNAME.s.html; \
            cat2html $TESTBASE.s ./html/$TESTNAME.s.html; \
            mv ./html/$TESTNAME.s.html ./html/$TESTPREFIX.$TESTNAME.s.html; \
	fi;	\
	fi;

	if test -s "$RESULT"; then \
		echo "<hr>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
		echo "<p><h2>target output:</h2>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
        cat2html $RESULT ./html/$TESTPREFIX.$TESTNAME.diff.html; \
	fi;

	if test "$DFLAG" -eq "1" -o "$TERR" -eq "1"; then \
	if test -s "$REFERENCE"; then \
		echo "<hr>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
		echo "<h2>reference output:</h2>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
        cat2html $REFERENCE ./html/$TESTPREFIX.$TESTNAME.diff.html;\
	fi;	\
	fi;

	if test "$DFLAG" -eq "1"; then \
		echo "<hr>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
		echo "<h2>output differences</h2>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
		echo "target | reference<br><br><tt>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
		diff --suppress-common-lines -w -y -B -b $RESULT $REFERENCE | \
    		sed -e 's/ /\&nbsp;/g' \
			    -e 's/$/<br>/' \
			>> ./html/$TESTPREFIX.$TESTNAME.diff.html; \
		echo "</tt>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
	fi;

#		diff -y -B -b $RESULT $REFERENCE | \
#			grep -i "|" | \


	echo "<hr>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html

	if test -s ./temp/$TESTNAME.errors; then
        echo "<h2>target compiler errors:</h2>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
        cat2html ./temp/$TESTNAME.errors ./html/$TESTPREFIX.$TESTNAME.diff.html
	fi;

	if test -s ./temp/$TESTNAME.warnref; then
        echo "<h2>reference compiler errors:</h2>" >> ./html/$TESTPREFIX.$TESTNAME.diff.html
        cat2html ./temp/$TESTNAME.warnref ./html/$TESTPREFIX.$TESTNAME.diff.html
	fi;

}

#
# create report
#

export TESTPREFIX=$TARGETSTD-o$TARGETOPTS

reporttop

echo "<table bgcolor=\"#a0a0a0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
echo "<table bgcolor=\"#a0a0a0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE

echo "<h1>automatic compiler testsuite "$VERSION"</h1>" >> $OUTFILE
echo "<small>"$COPYRIGHT"</small>" >> $OUTFILE

echo "</td></tr></table>" >> $OUTFILE
echo "<br></td></tr></table>" >> $OUTFILE


echo "<p><a href=\"./index.html\">Index</a>" >> $OUTFILE
echo " <a href=\"./report-overview.html\">Overview</a>" >> $OUTFILE
echo " List" >> $OUTFILE

echo "<h2>results:</h2>" >> $OUTFILE

echo "<table bgcolor=\"#c0c0c0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE
echo "<table bgcolor=\"#c0c0c0\" align=\"center\" width=\"99%\"><tr><td>" >> $OUTFILE

echo "<b>target:</b>" $TARGET >> $OUTFILE
echo "&nbsp&nbsp&nbsp<b>standard:</b>" $TARGETSTD >> $OUTFILE
echo "&nbsp&nbsp&nbsp<b>target compiler:</b>" $TARGETCC >> $OUTFILE
echo "("` cat ./temp/targetcc.version `")"  >> $OUTFILE
echo "&nbsp&nbsp&nbsp<b>options:</b>" $TARGETOPTS >> $OUTFILE
echo "&nbsp&nbsp&nbsp<b>reference compiler:</b>" $REFCC >> $OUTFILE

echo "<br><br><b>options:</b>&nbsp&nbsp&nbsp" >> $OUTFILE
grep -i "#define" ./arch/$TARGET/targetconf.h | grep -v "//" | grep -v "/\*" | sed -e 's/#define/\&nbsp\;\&nbsp\;/g' >> $OUTFILE

echo "<br><br><b>reference options:</b>&nbsp&nbsp&nbsp" >> $OUTFILE
grep -i "#define" ./arch/$REFCC/refconfig.h | grep -v "//" | grep -v "/\*" | sed -e 's/#define/\&nbsp\;\&nbsp\;/g' >> $OUTFILE

echo "</td></tr></table>" >> $OUTFILE
echo "<br></td></tr></table>" >> $OUTFILE

for I in $TESTS ; do \
    TESTBASE=$I; \
    i=`echo "$I" | sed -e 's/.*\///'`; \
	echo -n "$i,"; \
	TESTNAME=$i; \
	RESULT=""; \
	REFERENCE=""; \
	DFLAG="0"; \
    TERR="0"; \
    HERR="0"; \
	TESTCOLOR="#ffc0c0"; \
    TESTTEXT=""; \
    if test -s ./arch/$TARGETCC/$i.ref.$TARGETCC; then \
        REFEXT="ref.$TARGETCC"
        REFERENCE=./arch/$TARGETCC/$i.ref.$TARGETCC; \
    else \
        REFEXT="ref"
        if test -s ./temp/$i.ref; then \
            REFERENCE=./temp/$i.ref; \
        fi;\
    fi;\
	if test -s ./temp/$i.result; then \
		RESULT=./temp/$i.result; \
		if test -s "$REFERENCE"; then \
			if diff -q -w -B -b $RESULT $REFERENCE > /dev/null ; then \
				TESTCOLOR="#c0ffc0"; \
			else \
				DFLAG="1"; \
				TESTTEXT="errors in target output"; \
                TERR="1"; \
			fi;\
		else \
			TESTTEXT="reference output missing (did not compile for host?)"; \
            HERR="1"; \
		fi;\
	else \
		if test -s "$REFERENCE"; then \
    		TESTTEXT="target output missing  (did not compile for target?)"; \
		else \
			TESTTEXT="reference output missing (did not compile for host?)"; \
            HERR="1"; \
		fi;\
        TERR="1"; \
	fi;\
	testshort; \
	testlong; \
	echo $TERR""$HERR""$DFLAG > ./temp/$TESTPREFIX.$TESTNAME.status ;\
	done

rm -f *.result

reportbottom


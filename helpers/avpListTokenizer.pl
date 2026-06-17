#!/usr/bin/perl

open (RFC, "< ./RFC4006.txt") || die "Cannot open file";
our $avpCount = 0;

while (<RFC>) 
  {
    my $line = $_;
    $line =~ s/\n//g;
    my @tokens = split (/\;/, $line);
    my $firstString = $tokens[0];
    my $firstStringLen = length ($firstString);
    my $in = rindex ($firstString, '-');
    my $avpCode = $tokens[1];
    my $sectionDefined = $tokens[2];
    my $dataType = $tokens[3];
    my $must = $tokens[4];
    my $may = $tokens[5];
    my $shldNot = $tokens[6];
    my $mustNot = $tokens[7];
    my $encr = $tokens[8];
    my $name;

    $avpCount ++;

    if ($in == ($firstStringLen -1)) 
      {
	#print "$firstString is hyphenated!: ($tokens[1])\n";
	$line = <RFC>;
	$line =~ s/\n//g;
	@tokens = split (/\;/, $line);
	$name = $firstString . $tokens[0];
 
     }
    else 
      {
	$name = $firstString;
      }
    my $MUST = getFlags ($must, "must");
    my $MAY  = getFlags ($may, "may");
    my $SHLDNOT = getFlags ($shldNot, "shouldNot");
    my $MUSTNOT = getFlags ($mustNot, "mustNot");
    my $ENCR = getFlags ($encr, "encr");
    #print "BasicString name$avpCount (\"$name\");\n";
    #print "addAvp (name$avpCount, $avpCode, $dataType, \n\t$MUST, /**< must */";
    print "addAvp ((char *)\"$name\", $avpCode, $dataType, \n\t$MUST, /**< must */";
    print "\n\t$MAY, /**< may */\n\t$SHLDNOT, /**< should not */";
    print "\n\t$MUSTNOT, /**< must not */\n\t$ENCR /**< encryption */\n\t);\n";
  }
close (RFC);

sub getFlags ()
  {
    my $args = shift;
    my $flagType = shift;
    my @flagTokens = split (',', $args);
#     print " received ($args) : " . join ("::", @flagTokens);
#     print "\n";
    my $compositeFlag;

    if ($flagTokens[0] eq "P") 
      {
	$compositeFlag = "DIAMETER_AVP_PBIT_FLAG";
      }
    elsif ($flagTokens[0] eq "V")
      {
	$compositeFlag = "DIAMETER_AVP_VENDOR_SPECIFIC_FLAG";
      }
    elsif ($flagTokens[0] eq "M")	
      {
	$compositeFlag = "DIAMETER_AVP_MANDATORY_FLAG";
      }
    elsif ($flagTokens[0] eq "Y")
      {
	$compositeFlag = "true";
      }
    elsif ($flagTokens[0] eq "N")
      {
	$compositeFlag = "false";
      }
    elsif ($flagTokens[0] eq "INVALID")
      {
	    $compositeFlag = "DIAMETER_AVP_INVALID_FLAG"; # nothing is set
      }

    for (my $i = 1; $i <= $#flagTokens; $i++) 
      {
	if ($flagTokens[$i] eq "P") 
	  {	    
	    $compositeFlag .= " | DIAMETER_AVP_PBIT_FLAG";
	  }
	elsif ($flagTokens[$i] eq "V")
	  {
	    $compositeFlag .= " | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG";
	  }
	elsif ($flagTokens[$i] eq "M")	
	  {
	    $compositeFlag .= " | DIAMETER_AVP_MANDATORY_FLAG";
	  }
	elsif ($flagTokens[$i] eq "Y")
	  {
	    $compositeFlag = "ERROR";
	  }
	elsif ($flagTokens[$i] eq "N")
	  {
	    $compositeFlag = "ERROR";
	  }
	elsif ($flagTokens[$i] eq "INVALID")
	  {
	    $compositeFlag = "ERROR";
	  }
	else 
	  {	    
	  }
      }
    return $compositeFlag;
 }

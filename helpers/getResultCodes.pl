#!/usr/bin/perl

open (RES, "< rescode.txt") || die "cannot open the file";

while (<RES>) 
  {
    my $line = $_;
    $line =~ s/\n//g;

    my ($code, $str) = split (/\s+/, $line);
    my $comments = substr $line, (index ($line, $str) + length ($str));
    $comments =~s/\t//g;
    print "#define $str\t\t$code /**< $comments */\n";
  }

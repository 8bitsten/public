
#!/usr/bin/perl

# -------------------------------------------------------------------------------------------
#
# Modifies the output from https://petscii.krissz.hu/ PETSCII editor so those can be used 
# in C64 BASIC
# 
# Note: This is quick and dirty solution. I have never used the Perl before and I cannot
#       install any modules with CPAN
#
# -------------------------------------------------------------------------------------------
#
# Petr Mazanec
# petr.mazanec@gmail.com
# https://twitter.com/8bitsten/
#
# -------------------------------------------------------------------------------------------

use strict;
use warnings;

my $line_number = 1000;
my $EOL = "\r\n";

my ($screen_file_name, $color_file_name, $output_file_name) = @ARGV;

if (not defined $screen_file_name) 
{
    die $0.": Screen file name is missing (https://petscii.krissz.hu export)\n";
}

if (not defined $color_file_name) 
{
    die $0.": Color file name is missing (https://petscii.krissz.hu export)\n";
}

if (not defined $output_file_name) 

{
  die $0.": Output file name is missing\n";
}

open( my $screen_file_handle, "<", $screen_file_name ) or die $!;
open( my $color_file_handle,   "<", $color_file_name ) or die $!;
open( my $output_file_handle, ">", $output_file_name ) or die $!;

while ( !eof( $screen_file_handle ) and !eof( $color_file_handle ) ) 
{
    my $screen_line = <$screen_file_handle>;
    my $color_line  = <$color_file_handle>;

    # Remove left and right white spaces
    $screen_line =~ s/^\s+|\s+$//g;
    $color_line  =~ s/^\s+|\s+$//g;

    # Remove the 'BYTE ' substring
    $screen_line =~ s/BYTE //ig;
    $color_line  =~ s/BYTE //ig;

    # Split line to fit the maximum of 80 bytes C64 basic line length
    my @screen_line_split = split(',', $screen_line);
    my @color_line_split  = split(',', $color_line);
    
    my $line_BASIC = "";
    my $chars_count = 1;
    for( my $i = 0; $i <= $#screen_line_split; $i++ )
    {
        if ( $chars_count % 8 != 0 )
        {
            $line_BASIC .= $screen_line_split[$i].",";
            $line_BASIC .= $color_line_split[$i].",";
        }
        else
        {
            # Print line
            chop ( $line_BASIC );
            $line_BASIC = $line_number." DATA ".$line_BASIC.$EOL; 
            $line_BASIC =~ s/   / /ig;
            $line_BASIC =~ s/  / /ig;
            $line_BASIC =~ s/, /,/ig;
            print { $output_file_handle } $line_BASIC;

            $line_number += 10;
            $chars_count = 1;
            $line_BASIC = $screen_line_split[$i].",";
            $line_BASIC .= $color_line_split[$i].",";
        }
        $chars_count++;
    }

    # Print remining data on line
    if ( $chars_count != 0 )
    {
        chop ( $line_BASIC );
        $line_BASIC = $line_number." DATA ".$line_BASIC.$EOL; 
        $line_BASIC =~ s/   / /ig;
        $line_BASIC =~ s/  / /ig;
        $line_BASIC =~ s/, /,/ig;
        print { $output_file_handle } $line_BASIC; 
        $line_number += 10; 
    }
}

close $screen_file_handle;
close $color_file_handle;
close $output_file_handle;
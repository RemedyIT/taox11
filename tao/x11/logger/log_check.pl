#---------------------------------------------------------------------
# @file   log_check.pl
# @author Marijke Hengstmengel
#
# @copyright Copyright (c) Remedy IT Expertise BV
#---------------------------------------------------------------------
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

$status = 0;
$category = @ARGV[0];

$found = 0;
my $char = ':';
open FILE, ($#ARGV > 0 ? "<".@ARGV[1] : "<output");
while (my $line = <FILE>)
{
 print $line;
 if ($line =~ "begin priority testcase:")
 {
   my $result = rindex($line, $char);
   my $testcase_nr = substr $line, ($result + 2);
   chomp  $testcase_nr;

   $found = 1;
   #next line
   $line = <FILE>;
   do
   {
     if ($line =~ "expected:")
     {
       my $result = rindex($line, $char);
       my $prio = substr $line, ($result + 2);
       chomp  $prio;

       $line = <FILE>;
       print "next line should be a $prio message:\n";
       if ($line =~ $prio  )
       {
         #ok expected macro found
         print "OK expected macro found: $line";
         #next line
         $line = <FILE>;
       }
       else
       {
         #error expected macro not found
         print "ERROR expected macro not found: <$prio>, but found $line \n";
         $found = 0;
         close FILE;
         last;
       }
     }
     if ($line =~ "not expected priority testcase: $testcase_nr")
     {
       #next line
       $line = <FILE>;
       if ($line =~ "end priority testcase: $testcase_nr")
       {
         #ok no unexpected macro found
         print "OK no unexpected macro found \n";
       }
       else
       {
         #error unexpected macro found
         print "ERROR unexpected macro found: $line \n";
         $found = 0;
         close FILE;
         last;
       }
     }
   } until ($line =~ "end priority testcase: $testcase_nr")

 }
 if ($line =~ "begin verbose testcase:")
 {
   my $result = rindex($line, $char);
   my $testcase_nr = substr $line, ($result + 2);
   chomp  $testcase_nr;

   $found = 1;
   $verbose_found = 1;
   #next line
   $line = <FILE>;
   do
   {
     if ($line =~ "verbosity expected:")
     {
       my $result = rindex($line, $char);
       my $verboses = substr $line, ($result + 2);
       chomp  $verboses;
       my $combi_id = 0;
       if (($verboses =~ 'V_THREAD_ID') && ($verboses =~ 'V_PROCESS_ID'))
       {
         $combi_id = 1;
       }


       @verbose_array = split(',',$verboses);

       print "next line should be a message with verbose values @verbose_array:\n";
       $line = <FILE>;
       print $line;

       foreach $verbose (@verbose_array)
       {
         if (($verbose eq 'V_THREAD_ID') || ($verbose eq 'V_PROCESS_ID'))
         {
           my $search_str =  "\([0-9]+\)";
           if ($combi_id == 1)
           {
             $search_str =  "\([0-9]+\|[0-9]+\)";
           }
           if ($line =~ $search_str )
           {
             #ok expected verbose found
             print "OK expected verbose $verbose found\n";
           }
           else
           {
             #error expected verbose not found
             print "ERROR expected verbose $verbose not found\n";
             $verbose_found = 0;
             last;
           }
         }
         elsif ($verbose eq 'V_CATEGORY')
         {
           if ($line =~ /^$category /)
           {
             #ok expected verbose found
             print "OK expected verbose CATEGORY $category found\n";
           }
           else
           {
             #error expected verbose not found
             print "ERROR expected verbose CATEGORY $category not found\n";
             $verbose_found = 0;
             last;
           }
         }
         elsif ($verbose eq 'V_PRIO')
         {
           if ($line =~ /\[LP_INFO\]/)
           {
             #ok expected verbose found
             print "OK expected verbose PRIO found\n";
           }
           else
           {
             #error expected verbose not found
             print "ERROR expected verbose PRIO not found\n";
             $verbose_found = 0;
             last;
           }
         }
         elsif ($verbose eq 'V_TIME')  #HH:MM:SS.ssssss
         {
           if ($line =~ /[0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]*/)
           {
             #ok expected verbose found
             print "OK expected verbose TIME found\n";
           }
           else
           {
             #error expected verbose not found
             print "ERROR expected verbose TIME not found\n";
             $verbose_found = 0;
             last;
           }
         }
         elsif ($verbose eq 'V_DATETIME') #YYYY-MM-DD HH:MM:SS.ssssss
         {
           if ($line =~ /[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]*/)
           {
             #ok expected verbose found
             print "OK expected verbose DATETIME found\n";
           }
           else
           {
             #error expected verbose not found
             print "ERROR expected verbose DATETIME not found\n";
             $verbose_found = 0;
             last;
           }
         }
         else
         {
           #error unknown verbose option expected
           print "ERROR: expected verbose option doesn't exist\n";
           $verbose_found = 0;
           last;
         }
       } #end foreach
     } #if expected

     if ($line =~ "verbosity unexpected:")
     {
       my $result = rindex($line, $char);
       my $verboses = substr $line, ($result + 2);
       chomp  $verboses;

       #next line should be a message without verbose values $verboses";
       print "expect no message with verbose values $verboses:\n";
       $line = <FILE>;

       if ($verboses =~ 'V_CATEGORY')
       {
         if ($line =~ /^$category /)
         {
           #error unexpected verbose found
           print "ERROR unexpected verbose CATEGORY found\n";
           print $line;
           $verbose_found = 0;
         }
       }
       if ($verbose =~ 'V_PRIO')
       {
         if ($line =~ /\[LP_INFO\]/)
         {
           #error unexpected verbose found
           print "ERROR unexpected verbose CATEGORY found\n";
           print $line;
           $verbose_found = 0;
         }
       }
       if ($verbose =~ 'V_TIME')  #HH:MM:SS.ssssss
       {
         if ($line =~ /[0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]*/)
         {
           #error unexpected verbose found
           print "ERROR unexpected verbose TIME found\n";
           print $line;
           $verbose_found = 0;
         }
       }
       if ($verbose =~ 'V_DATETIME') #YYYY-MM-DD HH:MM:SS.ssssss
       {
         if ($line =~ /[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]*/)
         {
            #error unexpected verbose found
            print "ERROR unexpected verbose DATETIME found\n";
            print $line;
            $verbose_found = 0;
         }
       }
       if ($verbose_found > 0)
       {
         print "OK no unexpected verbose options found.\n";
       }
     } #if unexpected

     #sofar no error , go to next line
     if ($verbose_found > 0)
     {
       #next line
       $line = <FILE>;
     }
     else
     {
       $found = 0;
       close FILE;
       last;
     }

   } until ($line =~ "end verbose testcase: $testcase_nr")

 }


 if ($line =~ "begin same message testcase:")
 {
   my $result = rindex($line, $char);
   my $testcase_nr = substr $line, ($result + 2);
   chomp  $testcase_nr;

   $found = 1;
   $same_found = 1;
   #next line
   $line = <FILE>;
   do
   {
     if ($line =~ "same message expected:")
     {
       my $result = rindex($line, $char);
       my $message = substr $line, ($result + 2);
       chomp  $message;

       print "next 2 lines should be a message with <$message>:\n";
       $line = <FILE>;
       print $line;
       if (($line =~ /$message$/) && ($line =~ /^$category/))
       {
         #ok first line is as expected, now get next line
         $next_line = <FILE>;
         print $next_line;
         if ($line eq $next_line)
         {
            #ok expected two equal messages found
            print "OK expected equal messages found\n";
         }
         else
         {
           #error expected same messages not found
           print "ERROR expected same messages not found\n";
           $same_found = 0;
         }
       }
       else
       {
         #error expected message not found
         print "ERROR expected message not found\n";
         $same_found = 0;
       }
     } #if expected

     #sofar no error , go to next line
     if ($same_found > 0)
     {
       #next line
       $line = <FILE>;
     }
     else
     {
       $found = 0;
       close FILE;
       last;
     }
   } until ($line =~ "end same message testcase: $testcase_nr")
 }

 if ($line =~ "begin 2 in 1 message testcase:")
 {
   my $result = rindex($line, $char);
   my $testcase_nr = substr $line, ($result + 2);
   chomp  $testcase_nr;

   $found = 1;
   $lines_found = 1;
   #next line
   $line = <FILE>;
   do
   {
     if ($line =~ "2 lines expected:")
     {
       my $result = rindex($line, $char);
       my $message = substr $line, ($result + 2);
       chomp  $message;
       $result = rindex($message, '&');
       my $line1 = substr $message, 0, ($result - 1);
       my $line2 = substr $message, ($result + 2);

       print "next 2 lines should be <$line1> and <$line2>:\n";
       $line = <FILE>;
       print $line;
       if (($line =~ /$line1$/) && ($line =~ /^[$category]/))
       {
         #ok first line is as expected, now get next line
         $next_line = <FILE>;
         print $next_line;
         if ($next_line =~ /$line2$/)
         {
            #ok expected two lines found
            print "OK expected lines found\n";
         }
         else
         {
           #error expected lines not found
           print "ERROR expected lines not found\n";
           $lines_found = 0;
         }
       }
       else
       {
         #error expected line not found
         print "ERROR expected line not found\n";
         $lines_found = 0;
       }
     } #if expected

     #sofar no error , go to next line
     if ($lines_found > 0)
     {
       #next line
       $line = <FILE>;
     }
     else
     {
       $found = 0;
       close FILE;
       last;
     }
   } until ($line =~ "end 2 in 1 message testcase: $testcase_nr")
 }

}

print "\nFound: $found \n";
if ($found>0) {
  print " OK: Log output as expected.\n";
} else {
  print STDERR " ERROR: Unable to detect expected log output.\n";
  $status = 1;
}
exit $status;

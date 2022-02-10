Logging  test.

In this test logging macro's of TAOX11 and the base X11 are used.

Different settings of the priority masks and verbosity masks of TAOX11_LOGGER are used
after each other. For this the setter operations of the priority_mask and verbosity_mask are used.
The priority mask determine which macro's are enabled.
The verbosity mask  determine the format of the output of the macro's.

We check automatically the right output of the macro's in the script $TAOX11_ROOT/tao/x11/logger/log_check.pl
The script expected a blocks of code in a special order.  This is described in
$TAOX11_ROOT/tao/x11/logger/readme.txt

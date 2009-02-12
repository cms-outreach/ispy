while (<>) {
    if (/^\%\%Pages: (\d+)/) {
	print '%%Pages: ' . ($1 + 1) . "\n";
    }
    elsif (/^\%\%EndSetup/) {
	print; print '%%Page: 1 1', "\nshowpage\n";
    }
    elsif (/^\%\%Page: (\d+) (\d+)/) {
	print '%%Page: ' . ($1+1) . ' ' . ($2+1) . "\n";
    }
    else {
	print;
    }
}

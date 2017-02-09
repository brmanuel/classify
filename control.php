<?php

echo("training [1] or classification [0]?");
$training = intval(fgets(STDIN));

echo("address: \n");
$address = fgets(STDIN);

echo("password: ");
$password = fgets(STDIN);

// connection to hotmail server. will not work for other email clients (change server name -> imap-mail.outlook.com and port -> 993
$imap = imap_open("{imap-mail.outlook.com:993/ssl/novalidate-cert}", $address, $password);

if( $imap ) {

     $num = imap_num_msg($imap);
     
     if( $num >0 ) {

	 $latest_uid = intval(file_get_contents("uids.txt"));

     	 if ($latest_uid) $mail = imap_search($imap, 'UID ' . $latest_uid . ':*', SE_UID);
	 else $mail = imap_search($imap, 'ALL');

	 $result = imap_fetch_overview($mbox, $num, 0);
	 $latest_uid = $result[0]->uid;
	 file_put_contents("uids.txt", $latest_uid);
	 
	 foreach($mail as $email) {
	 	$header = imap_headerinfo($imap, $email)->subject;
				 
	 	$target = quoted_printable_decode(imap_fetchbody($imap, $email, 1));
		if ($target == '') $target = quoted_printable_decode(imap_fetchbody($imap, $email, 1.1));
		if ($target == '') continue;

		$name = $header . '.txt';
		file_put_contents ($name, $target);
		if ($training) {
		   echo ($target . '\nspam [1] or honest [0]?');
		   $spam = intval(fgets(STDIN));
		   shell_exec('./classify ' . $name . $spam);
		}
		else {
		   shell_exec('./classify ' . $name);  
		}
	}
     }
     
     imap_close($imap);
}

?>


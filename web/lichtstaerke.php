<?php
	// Die Klasse f�r den Datenbank-Zugriff wird ben�tigt
	require('classes/databasehandler.php');
?>

<html>
	<head>
		<meta content="text/html; charset=iso-8859-1" http-equiv="Content-Type">
		<link rel="stylesheet" type="text/css" href="site.css">
		<title>Raspberry Pi - Wetterstation - Lichtst�rke</title>
	</head>
	<body>
		<?php include("menu.php"); ?>
		
		<h2>Lichtst�rke</h2>
		
		<p>Aktuelle Lichtst�rke: <?php echo DatabaseHandler::getLastAmbientLight(); ?></p>
		
		<p><img src="lichtstaerke_graph.php" alt="[Die letzten Temperatur-Messungen]"></p>
	</body>
</html>

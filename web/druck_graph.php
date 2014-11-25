<?php
	// Die Klassen f&#65533;r den Graphen werden ben&#65533;tigt
	require_once ("/usr/share/jpgraph/jpgraph.php");
	require_once ("/usr/share/jpgraph/jpgraph_line.php");
	
	// Die Datenbank &#65533;ffnen
	$dbFileName = "/var/piweather/piweather.sqlite3";	
	$db = new SQLite3($dbFileName);
	
	// Die Temperatur-Eintr&#65533;ge ermitteln
	$query = "SELECT * FROM piw_values ORDER BY id DESC LIMIT 10";
	$results = $db->query($query);
	
	// Die Werte der Y-Achse definieren
	$labels = array();
	$yValues = array();
	while ($row = $results->fetchArray()) {
		$labels[] = date("d.m.y", $row['created_at']) . "\n" . date("H:i", $row['created_at']);
		$yValues[] = $row['pressure'];
	}	
	
	// Die Datenbank wieder schliessen
	$db->close();
	
	// Den Graphen erstellen
	$graph = new Graph(600,300,"auto");   	
	$graph->SetScale("textlin");
	$graph->img->SetMargin(40, 20, 20, 40);
	$graph->xaxis->SetTickLabels(array_reverse($labels));
	
	// Den Lineplot erstellen
	$lineplot=new LinePlot(array_reverse($yValues));
	$lineplot->SetColor("blue");
	$lineplot->SetWeight(2); 
	$graph->Add($lineplot);
	
	// Den Graphen zeichnen
	$graph->Stroke();	
?>

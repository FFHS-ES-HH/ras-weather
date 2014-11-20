<?php
	require_once ("/usr/share/jpgraph/jpgraph.php");
	require_once ("/usr/share/jpgraph/jpgraph_line.php");

	$graph = new Graph(600,300,"auto");   	
	
	// Die Daten des Graphen
	$graph->SetScale("textlin");
	$ydata = array(11,3,8,12,5,1,9,13,5,7); 
	$lineplot=new LinePlot($ydata);
	$graph->Add($lineplot);
	
	// Die Bezeichnungen des Graphen
	$graph->title->Set("Die letzten Temperatur-Messungen");
	$graph->title->SetFont(FF_FONT1,FS_BOLD);
	$graph->xaxis->title->Set("Messungen");
	$graph->yaxis->title->Set("Temperatur [°C]");
	$graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
	$graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);
	$graph->img->SetMargin(40,20,20,40);
	
	$lineplot->SetColor("blue");
	$lineplot->SetWeight(2); 
	$graph->SetShadow(); 
	
	$graph->Stroke();		
?>

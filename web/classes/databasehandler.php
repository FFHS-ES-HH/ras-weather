<?php
	class DatabaseHandler
	{
		// Die letzte Temperatur ermitteln
		public static function getLastTemperature()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0)
			{
				$message = round($row['temperature'], 2) . " [°C] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}
		
		// Die letzte Lichtstärke ermitteln
		public static function getLastIlluminance()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0)
			{
				$message = round($row['illuminance'], 2) . " [lx] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}

		// Die letzte Luftfeuchtigkeit ermitteln
		public static function getLastHumidity()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0)
			{
				$message = round($row['humidity'], 2) . " [%] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}

		// Der letzte Luftdruck ermitteln
		public static function getLastPressure()
		{
			// Die Datenbank öffnen
			$dbFileName = "/var/piweather/piweather.sqlite3";	
			$db = new SQLite3($dbFileName);
			
			// Den letzten Eintrag ermitteln
			$query = "SELECT * FROM piw_values ORDER BY id DESC";
			$results = $db->query($query);
			$row = $results->fetchArray();

			// Prüfen, ob zumindest eine Zeile zurückgegeben wurde
			$message = "";
			if(count($row)>0)
			{
				$message = round($row['pressure'], 2) . " [hPa] - " . date("d.m.Y", $row['created_at']) . " (" . date("H:i", $row['created_at']) . ")";
			}
			else
			{
				$message = "Bisher wurde noch keine Messung durchgeführt.";
			}
			
			// Die Datenbank wieder schliessen
			$db->close();
					
			// Die Meldung zurückgeben
			return $message;	
		}
	}
?>

<html>
<head>
<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
</head>
<body></body>
</html>

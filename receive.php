<?php

$sensor = "none";
if (isset($_GET["sensor"]))
	$sensor = $_GET["sensor"];

if ($sensor == "ds" && isset($_GET["temp"]))
	file_put_contents ("ds-last-value.txt" , $_GET["temp"]);
?>

<?php

/**
 * resize_brand_image.inc
 * 
 * @version 2013-04-29
 * @author A.Alkaissi <info@buero-hahn.de>
 */
$img = new Image();
$cmt = Contentomat::getContentomat();

foreach ($_FILES as $fieldName => $file) {

	$fileInfo = @ pathinfo($file['name']);
	$fileExtension = strtolower($fileInfo['extension']);

	if ($fieldName == 'brands_logo_newfile') {
		// Gibt's ne Datei?
		if ($file['name']) {

			$fileNameParts = explode(".", $file['name']);
			$fileExtension = array_pop($fileNameParts);
			$fileNameBody = $cmt->makeNameWebSave(join(".", $fileNameParts));
			$newFilename = $fileNameBody . '.' . $fileExtension;

			// 250
			$img->createThumbnail(array('sourceImage' => $file['tmp_name'],
				'sourceImageType' => $fileExtension,
				'destinationImage' => PATHTOWEBROOT . 'img/logos/250/' . $newFilename,
				'destinationImageType' => $fileExtension,
				'width' => 250
			));


			// 200
			$img->createThumbnail(array('sourceImage' => $file['tmp_name'],
				'sourceImageType' => $fileExtension,
				'destinationImage' => PATHTOWEBROOT . 'img/logos/200/' . $newFilename,
				'destinationImageType' => $fileExtension,
				'width' => 200
			));


			// 100
			$img->createThumbnail(array('sourceImage' => $file['tmp_name'],
				'sourceImageType' => $fileExtension,
				'destinationImage' => PATHTOWEBROOT . 'img/logos/100/' . $newFilename,
				'destinationImageType' => $fileExtension,
				'width' => 100
			));
			
			// 84
			$img->createThumbnail(array('sourceImage' => $file['tmp_name'],
				'sourceImageType' => $fileExtension,
				'destinationImage' => PATHTOWEBROOT . 'img/logos/84/' . $newFilename,
				'destinationImageType' => $fileExtension,
				'width' => 84
			));
			
			// 60
			$img->createThumbnail(array('sourceImage' => $file['tmp_name'],
				'sourceImageType' => $fileExtension,
				'destinationImage' => PATHTOWEBROOT . 'img/logos/60/' . $newFilename,
				'destinationImageType' => $fileExtension,
				'width' => 60
			));
		}
	}
}
?>

#include <iostream> // std::cout, std::cin
#include <string> // std::string
#include <itkImage.h> // itk::Image

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <itkRescaleIntensityImageFilter.h>

using ImageType = itk::Image<short, 3>;
using ImageType2D = itk::Image<short, 2>;
using ReaderType = itk::ImageFileReader<ImageType>;
using WriterType = itk::ImageFileWriter<ImageType>;

using SeriesReaderType = itk::ImageSeriesReader<ImageType>;
using SeriesWriterType = itk::ImageSeriesWriter<ImageType, ImageType2D>;

void SaveImage(ImageType::Pointer image, std::string name) {
	using WriterType = itk::ImageFileWriter<ImageType>;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(image);
	writer->SetFileName(name);
	writer->Update();
}

ImageType::Pointer ReadImage(std::string path) {
	itk::GDCMSeriesFileNames::Pointer gdcmNames = itk::GDCMSeriesFileNames::New();
	gdcmNames->SetDirectory(path);
	itk::SerieUIDContainer series = gdcmNames->GetSeriesUIDs();
	itk::FilenamesContainer fileNames = gdcmNames->GetFileNames(series[0]);
	SeriesReaderType::Pointer seriesReader = SeriesReaderType::New();
	seriesReader->SetFileNames(fileNames);
	seriesReader->Update();


	return seriesReader->GetOutput();
}

ImageType::Pointer RescaleInensity(ImageType::Pointer image) {
	using FilterType = itk::RescaleIntensityImageFilter<ImageType, ImageType>;
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(image);
	filter->SetOutputMaximum(255);
	filter->SetOutputMinimum(0);
	filter->Update();

	return filter->GetOutput();
}

int main() // glowna funkcja programu
{
	try {
		
		// 1. odczyt serii
		ImageType::Pointer image3D = ReadImage("../dane/p1/MR");
		// 2. przeskalowanie intensywnoœci
		ImageType::Pointer rescaledImage = RescaleInensity(image3D);
		// 3. zapis do kostki
		SaveImage(rescaledImage, "../wyniki/p1/MR.vtk");

	}
	catch (itk::ExceptionObject& ex) {
		ex.Print(std::cout);
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error!" << std::endl;
	}
	std::cout << "Hit [Enter]...";
	std::cin.get();
	return EXIT_SUCCESS; // albo EXIT_FAILURE
}

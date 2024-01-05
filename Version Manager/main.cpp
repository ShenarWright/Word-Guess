#include <iostream>
#include <fstream>

#include <TGUI/TGUI.hpp>
#include <libzippp/libzippp.h>

#include <cpr/cpr.h>
#include <json/json.h>
#include <teemo/teemo.h>

void copyFile(const std::string& from, const std::string& to)
{
	std::ifstream is(from, std::ios::in | std::ios::binary);
	std::ofstream os(to, std::ios::out | std::ios::binary);

	std::copy(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(os));
}

void SaveFileFromeBuffer(std::string buffers, std::string to)
{
	std::string buffer = buffers;
	std::ofstream os(to, std::ios::out | std::ios::binary);

	std::copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(os));
}

std::string getDirecTory(std::string str)
{
	std::string buffer;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '/')
		{
			break;
		}

		buffer += str[i];
	}
	std::cout << "BUFFER:" << buffer << '\n';
	return buffer;
}

int main(int argc,char* argv[])
{
	if (argc < 2)
	{
		std::cout << "NO INPUT\n";
		//return -1;
	}
	/*
	libzippp::ZipArchive zp("archive.zip");
	zp.open();
	

	zp.setErrorHandlerCallback([](const std::string& message,
		const std::string& strerror,
		int zip_error_code,
		int system_error_code)
		{
			// Handle error here
			fprintf(stderr, message.c_str(), strerror.c_str());
		});
	auto names = zp.getEntries();

	bool first = true;
	for (auto& e : names)
	{
		if (first)
		{
			if(tgui::Filesystem::createDirectory("unzip/" + getDirecTory(e.getName())))
				std::cout << "Created: " << getDirecTory(e.getName()) << '\n';

			first = false;
		}

		std::cout << e.getName() << '\n';
		auto buf = zp.getEntry(e.getName());
		
		if (e.isDirectory())
		{
			if (tgui::Filesystem::createDirectory("unzip/" + e.getName()))
				std::cout << "Created\n";
			continue;
		}
			

		std::ofstream ofs("unzip/" + e.getName(),std::ios::binary);
		
		if (!ofs.is_open())
			std::cout << "NOT OPENED\n";

		buf.readContent(ofs);
		ofs.close();
	}


	//copyFile("file.txt", "newfile.txt");
	zp.close();
	*/
	auto res = cpr::Get(cpr::Url("https://api.github.com/repos/bossx3255/Word-Guess/releases/latest"));
	//std::cout << res.text << '\n';
	Json::Value value;
	Json::Reader reader;
	reader.parse(res.text, value);
	std::cout << value << '\n';

	for (int i = 0; i < value["assets"].size(); i++)
	{
		std::cout << "DOWNLOAD: " << value["assets"][i]["browser_download_url"].asString();
	}

	teemo::Teemo teemo;
	auto responce = teemo.start(value["assets"][0]["browser_download_url"].asString(), "file.zip", 
		[](teemo::Result res) 
		{

		}, 
		[](int64_t total, int64_t downloaded) 
		{
			std::cout << "Total: " << total << '\n';
			std::cout << "Downloaded: " << downloaded << '\n';
			std::cout << ((float)downloaded / (float)total) * 100.f << "%\n";
		},
		[](int64_t byte_per_secs) 
		{
			
		});
	responce.wait();

	//SaveFileFromeBuffer(buf.readAsText(), "watch.png");
}
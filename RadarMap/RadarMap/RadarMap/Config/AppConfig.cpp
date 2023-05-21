#include "StdAfx.h"
#include "AppConfig.h"

void RadarMap::Config::AppConfig::Load(const char* fileName)
{
	char wd[MAX_PATH] = ""; 
	_getcwd(wd, MAX_PATH);
	strcat_s(wd, MAX_PATH, "\\");
	strcat_s(wd, MAX_PATH, fileName);

	PostProcessing.Shading = GetPrivateProfileIntA("PostProcessing", "Shading", 1, wd);
	PostProcessing.SpeckleNoise = GetPrivateProfileIntA("PostProcessing", "Speckle", 1, wd);
	PostProcessing.BeamPattern = GetPrivateProfileIntA("PostProcessing", "BeamPattern", 1, wd);
	PostProcessing.AdditiveNoise = GetPrivateProfileIntA("PostProcessing", "AdditiveNoise", 1, wd);

	char str[MAX_PATH] = "";	
	GetPrivateProfileStringA("HDDRate", "Geometry", "0.004", str, MAX_PATH, wd);
	HDDRate.Geometry = atof(str);
	GetPrivateProfileStringA("HDDRate", "Textures", "0.004", str, MAX_PATH, wd);
	HDDRate.Textures = atof(str);

	GetPrivateProfileStringA("LoadOnStartup", "Terrain", "", str, MAX_PATH, wd);
	LoadOnStartup.Terrain = str;
	GetPrivateProfileStringA("LoadOnStartup", "Targets", "", str, MAX_PATH, wd);
	LoadOnStartup.Targets = str;

	Debug.Trace = GetPrivateProfileIntA("Debug", "Trace", 0, wd);
	Debug.StaticRun = GetPrivateProfileIntA("Debug", "StaticRun", 0, wd);
	Debug.StaticWindow = GetPrivateProfileIntA("Debug", "StaticWindow", 0, wd);
}
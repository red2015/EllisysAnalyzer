<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="14008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="TestFindingAnalyzer" Type="Folder">
			<Item Name="Tests.vi" Type="VI" URL="../../USBAnalyzerEllisysDll/Debug/Tests.vi"/>
		</Item>
		<Item Name="FindAnalyzer.vi" Type="VI" URL="../../USBAnalyzerEllisysDll/Debug/FindAnalyzer.vi"/>
		<Item Name="GetSerialNumber.vi" Type="VI" URL="../../USBAnalyzerEllisysDll/Debug/GetSerialNumber.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="USBAnalyzerEllisysDll.dll" Type="Document" URL="../../USBAnalyzerEllisysDll/Debug/USBAnalyzerEllisysDll.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>

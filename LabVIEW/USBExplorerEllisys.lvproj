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
		<Item Name="AnalyzerStuff" Type="Folder">
			<Item Name="Acqusiton.vi" Type="VI" URL="../Acqusiton.vi"/>
			<Item Name="FindAnalyzer.vi" Type="VI" URL="../FindAnalyzer.vi"/>
			<Item Name="GetSerialNumber.vi" Type="VI" URL="../GetSerialNumber.vi"/>
			<Item Name="StopAcquisition.vi" Type="VI" URL="../StopAcquisition.vi"/>
		</Item>
		<Item Name="ChartRectangle" Type="Folder">
			<Item Name="ChartBar.vi" Type="VI" URL="../ChartBar.vi"/>
		</Item>
		<Item Name="TestFindingAnalyzer" Type="Folder">
			<Item Name="Tests.vi" Type="VI" URL="../Tests.vi"/>
		</Item>
		<Item Name="GetFrameStatistics.vi" Type="VI" URL="../GetFrameStatistics.vi"/>
		<Item Name="piechart.vi" Type="VI" URL="../piechart.vi"/>
		<Item Name="XY bar graph .vi" Type="VI" URL="../../../../Downloads/XY bar graph .vi"/>
		<Item Name="XyBarGraphTests.vi" Type="VI" URL="../XyBarGraphTests.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Draw Arc.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Arc.vi"/>
				<Item Name="Empty Picture" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Empty Picture"/>
				<Item Name="Set Pen State.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Set Pen State.vi"/>
			</Item>
			<Item Name="USBAnalyzerEllisysDll.dll" Type="Document" URL="../../USBAnalyzerEllisysDll/Debug/USBAnalyzerEllisysDll.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>

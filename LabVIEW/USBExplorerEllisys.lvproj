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
			<Item Name="DecreaseFrame.vi" Type="VI" URL="../DecreaseFrame.vi"/>
			<Item Name="FindAnalyzer.vi" Type="VI" URL="../FindAnalyzer.vi"/>
			<Item Name="GetDeviceTransactions.vi" Type="VI" URL="../GetDeviceTransactions.vi"/>
			<Item Name="GetFrameStatistics.vi" Type="VI" URL="../GetFrameStatistics.vi"/>
			<Item Name="GetSerialNumber.vi" Type="VI" URL="../GetSerialNumber.vi"/>
			<Item Name="StopAcquisition.vi" Type="VI" URL="../StopAcquisition.vi"/>
		</Item>
		<Item Name="Charts" Type="Folder">
			<Item Name="Chart stuff" Type="Folder">
				<Item Name="GetCountAllTransactions.vi" Type="VI" URL="../GetCountAllTransactions.vi"/>
				<Item Name="GetCountOfTransactionsInOutNak.vi" Type="VI" URL="../GetCountOfTransactionsInOutNak.vi"/>
				<Item Name="min(a,b).vi" Type="VI" URL="../min(a,b).vi"/>
			</Item>
			<Item Name="Example" Type="Folder">
				<Item Name="XY bar graph(1) .vi" Type="VI" URL="../XY bar graph(1) .vi"/>
				<Item Name="xy_bar_graph.vi" Type="VI" URL="../../../../Downloads/xy_bar_graph.vi"/>
				<Item Name="XyBarGraphTests.vi" Type="VI" URL="../XyBarGraphTests.vi"/>
			</Item>
			<Item Name="BarChart.vi" Type="VI" URL="../BarChart.vi"/>
			<Item Name="BarChart2.vi" Type="VI" URL="../BarChart2.vi"/>
			<Item Name="FrameChart.vi" Type="VI" URL="../FrameChart.vi"/>
			<Item Name="piechart.vi" Type="VI" URL="../piechart.vi"/>
		</Item>
		<Item Name="Tests" Type="Folder">
			<Item Name="ChartTesting.vi" Type="VI" URL="../ChartTesting.vi"/>
		</Item>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Draw Arc.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Arc.vi"/>
				<Item Name="Empty Picture" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Empty Picture"/>
				<Item Name="RGB to Color.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/colorconv.llb/RGB to Color.vi"/>
				<Item Name="Set Pen State.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Set Pen State.vi"/>
			</Item>
			<Item Name="USBAnalyzerEllisysDll.dll" Type="Document" URL="../../USBAnalyzerEllisysDll/Debug/USBAnalyzerEllisysDll.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>

using System;
using System.IO;
using System.Collections;

using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Ipc;
using System.Runtime.Remoting.Channels.Http;
using System.Runtime.Remoting.Channels.Tcp;

using System.Data;
using System.Data.OleDb;

using SysCAD.Interface;
using System.Runtime.Serialization.Formatters;

namespace SysCAD.Service
{
	class Service
	{
    [STAThread]
    static void Main(string[] args) 
    {
      BinaryClientFormatterSinkProvider clientProvider = null;
      BinaryServerFormatterSinkProvider serverProvider = new BinaryServerFormatterSinkProvider();
      serverProvider.TypeFilterLevel = TypeFilterLevel.Full;

      IDictionary ipcProps = new Hashtable();
      ipcProps["portName"] = "SysCAD.Service";
      ipcProps["typeFilterLevel"] = TypeFilterLevel.Full;
      IpcChannel ipcChannel = new IpcChannel(ipcProps, clientProvider, serverProvider);
      ChannelServices.RegisterChannel(ipcChannel, false);

      IDictionary tcpProps = new Hashtable();
      tcpProps["port"] = "8065";
      tcpProps["typeFilterLevel"] = TypeFilterLevel.Full;
      TcpChannel tcpChannel = new TcpChannel(tcpProps, clientProvider, serverProvider);
      ChannelServices.RegisterChannel(tcpChannel, false);

      IDictionary httpProps = new Hashtable();
      httpProps["port"] = "8066";
      httpProps["typeFilterLevel"] = TypeFilterLevel.Full;
      HttpChannel httpChannel = new HttpChannel(httpProps, clientProvider, serverProvider);
      ChannelServices.RegisterChannel(httpChannel, false);

      ConfigData config = new ConfigData();

      string args0;
      if (args.Length == 1)
        args0 = args[0];
      else
        args0 = "e:\\";

      foreach (string fullpath in Directory.GetFiles(args0, "*.modelstencil"))
      {
        System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(ModelStencil));
        TextReader reader = new StreamReader(fullpath);
        config.modelStencils.Add(Path.GetFileNameWithoutExtension(fullpath), (ModelStencil)ser.Deserialize(reader));
        reader.Close();
      }

      foreach (string fullpath in Directory.GetFiles(args0, "*.graphicstencil"))
      {
        System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(GraphicStencil));
        TextReader reader = new StreamReader(fullpath);
        config.graphicStencils.Add(Path.GetFileNameWithoutExtension(fullpath), (GraphicStencil)ser.Deserialize(reader));
        reader.Close();
      }


      string[] directoryList = Directory.GetFiles(args0, "*.mdb");

      foreach (string fullpath in directoryList)
      {
        string filename = Path.GetFileNameWithoutExtension(fullpath);

        GraphicData graphic = new GraphicData();
        graphic.name = filename;

        {
          OleDbConnection connection = new OleDbConnection(@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + fullpath);
          connection.Open();

          OleDbDataReader itemReader = (new OleDbCommand("SELECT DISTINCT Tag FROM GraphicsUnits", connection)).ExecuteReader(CommandBehavior.SingleResult);
          while (itemReader.Read())
          {
            Item item = new Item(itemReader.GetString(0));
            item.Populate(connection);
            graphic.items.Add(itemReader.GetString(0), item);
          }
          itemReader.Close();

          OleDbDataReader linkReader = (new OleDbCommand("SELECT DISTINCT Tag FROM ModelLinks", connection)).ExecuteReader(CommandBehavior.SingleResult);
          while (linkReader.Read())
          {
            Link link = new Link(linkReader.GetString(0));
            link.Populate(connection);
            graphic.links.Add(linkReader.GetString(0), link);
          }
          linkReader.Close();

          OleDbDataReader areaReader = (new OleDbCommand("SELECT DISTINCT Page FROM GraphicsUnits ORDER BY Page", connection)).ExecuteReader(CommandBehavior.SingleResult);
          float dX = 0.0F;
          while (areaReader.Read())
          {
            Area area = new Area(areaReader.GetString(0));
            area.Populate(connection, graphic.items, ref dX);
            graphic.___areas.Add(areaReader.GetString(0), area);
          }
          areaReader.Close();
        }

        RemotingServices.Marshal(graphic, filename);
        config.projectList.Add(filename);
        Console.WriteLine("Added project {0} to ProjectList.", filename);

      }

      RemotingServices.Marshal(config, "Global");

      Console.WriteLine();
      Console.Write("All projects added successfully, press <enter> to end.");
      Console.ReadLine();
    }	
  }
}

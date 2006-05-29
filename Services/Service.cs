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
using System.Drawing;

using SysCAD.Interface;
using System.Runtime.Serialization.Formatters;
using System.Runtime.Serialization.Formatters.Soap;
using System.Collections.Generic;

namespace SysCAD.Service
{
	class Service
	{
    private static bool CreateItem(ServiceGraphic graphic, uint requestID, Guid guid, String tag, String path, String model, String shape, RectangleF boundingRect, Single angle, System.Drawing.Color fillColor, bool mirrorX, bool mirrorY)
    {
      graphic.DoItemCreated(requestID, guid, tag, path, model, shape, boundingRect, angle, fillColor, mirrorX, mirrorY);
      return true;
    }

    private static bool ModifyItem(ServiceGraphic graphic, uint requestID, Guid guid, String tag, String path, String model, String shape, RectangleF boundingRect, Single angle, System.Drawing.Color fillColor, bool mirrorX, bool mirrorY)
    {
      graphic.DoItemModified(requestID, guid, tag, path, model, shape, boundingRect, angle, fillColor, mirrorX, mirrorY);
      return true;
    }

    private static bool DeleteItem(ServiceGraphic graphic, uint requestID, Guid guid)
    {
      graphic.DoItemDeleted(requestID, guid);
      return true;
    }

    private static bool CreateLink(ServiceGraphic graphic, uint requestID, Guid guid, String tag, String classID, Guid origin, Guid destination, String originPort, String destinationPort, List<PointF> controlPoints)
    {
      graphic.DoLinkCreated(requestID, guid, tag, classID, origin, destination, originPort, destinationPort, controlPoints);
      return true;
    }

    private static bool ModifyLink(ServiceGraphic graphic, uint requestID, Guid guid, String tag, String classID, Guid origin, Guid destination, String originPort, String destinationPort, List<PointF> controlPoints)
    {
      graphic.DoLinkModified(requestID, guid, tag, classID, origin, destination, originPort, destinationPort, controlPoints);
      return true;
    }

    private static bool DeleteLink(ServiceGraphic graphic, uint requestID, Guid guid)
    {
      graphic.DoLinkDeleted(requestID, guid);
      return true;
    }



    [STAThread]
    static void Main(string[] args) 
    {
      BinaryServerFormatterSinkProvider serverProv = new BinaryServerFormatterSinkProvider();
      serverProv.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;

      BinaryClientFormatterSinkProvider clientProv = new BinaryClientFormatterSinkProvider();

      IDictionary ipcProps = new Hashtable();
      ipcProps["portName"] = "SysCAD.Service";
      //ipcProps["typeFilterLevel"] = TypeFilterLevel.Full;
      IpcChannel ipcChannel = new IpcChannel(ipcProps, clientProv, serverProv);
      ChannelServices.RegisterChannel(ipcChannel, false);

      //IDictionary tcpProps = new Hashtable();
      //tcpProps["port"] = "8065";
      //tcpProps["typeFilterLevel"] = TypeFilterLevel.Full;
      //TcpChannel tcpChannel = new TcpChannel(tcpProps, null, null);
      //ChannelServices.RegisterChannel(tcpChannel, false);

      //IDictionary httpProps = new Hashtable();
      //httpProps["port"] = "8066";
      //httpProps["typeFilterLevel"] = TypeFilterLevel.Full;
      //HttpChannel httpChannel = new HttpChannel(httpProps, null, null);
      //ChannelServices.RegisterChannel(httpChannel, false);

      ConfigData config = new ConfigData();

      string args1;
      string args0;
      if (args.Length >= 2)
      {
        args1 = args[1];
        args0 = args[0];
      }
      else if (args.Length == 1)
      {
        args1 = ".\\";
        args0 = args[0];
      }
      else
      {
        args1 = ".\\";
        args0 = ".\\";
      }

      foreach (string fullpath in Directory.GetFiles(args1, "*.modelstencil"))
      {
        SoapFormatter sf = new SoapFormatter();
        Stream stream = new StreamReader(fullpath).BaseStream;
        ModelStencil modelStencil = (ModelStencil)sf.Deserialize(stream);
        modelStencil.Tag = Path.GetFileNameWithoutExtension(fullpath);
        config.modelStencils.Add(Path.GetFileNameWithoutExtension(fullpath), modelStencil);
        stream.Close();
        Console.WriteLine("Added modelstencil {0} to ProjectList.", Path.GetFileNameWithoutExtension(fullpath));
      }

      Console.WriteLine("\n");

      foreach (string fullpath in Directory.GetFiles(args1, "*.graphicstencil"))
      {
        SoapFormatter sf = new SoapFormatter();
        Stream stream = new StreamReader(fullpath).BaseStream;
        GraphicStencil graphicStencil = (GraphicStencil)sf.Deserialize(stream);
        graphicStencil.Tag = Path.GetFileNameWithoutExtension(fullpath);
        config.graphicStencils.Add(Path.GetFileNameWithoutExtension(fullpath), graphicStencil);
        stream.Close();
        Console.WriteLine("Added graphicstencil {0} to ProjectList.", Path.GetFileNameWithoutExtension(fullpath));
      }

      Console.WriteLine("\n");

      string[] directoryList = Directory.GetFiles(args0, "*.mdb");

      foreach (string fullpath in directoryList)
      {
        string filename = Path.GetFileNameWithoutExtension(fullpath);

        ServiceGraphic graphic = new ServiceGraphic(CreateItem, ModifyItem, DeleteItem, CreateLink, ModifyLink, DeleteLink);
        graphic.name = filename;

        {
          OleDbConnection connection = new OleDbConnection(@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + fullpath);
          connection.Open();

          OleDbDataReader itemReader = (new OleDbCommand("SELECT DISTINCT Tag FROM GraphicsUnits", connection)).ExecuteReader(CommandBehavior.SingleResult);
          while (itemReader.Read())
          {
            OleDbDataReader itemGuidReader = (new OleDbCommand("SELECT EqpGUID FROM ModelUnits WHERE Tag='" + itemReader.GetString(0) + "'", connection)).ExecuteReader();
            if (itemGuidReader.Read())
            {
              GraphicItem graphicItem = new GraphicItem(new Guid(itemGuidReader.GetString(0)), itemReader.GetString(0));
              graphicItem.Populate(filename, connection);
              graphic.graphicItems.Add(graphicItem.Guid, graphicItem);
            }
          }
          itemReader.Close();

          OleDbDataReader linkReader = (new OleDbCommand("SELECT DISTINCT Tag, EqpGUID FROM ModelLinks", connection)).ExecuteReader(CommandBehavior.SingleResult);
          while (linkReader.Read())
          {
            GraphicLink graphicLink = new GraphicLink(new Guid(linkReader.GetString(1)), linkReader.GetString(0));
            graphicLink.Populate(connection, graphic.graphicItems);
            graphic.graphicLinks.Add(graphicLink.Guid, graphicLink);
          }
          linkReader.Close();

          int pages = 0;
          OleDbDataReader pageCountReader = (new OleDbCommand("SELECT DISTINCT Page FROM GraphicsUnits ORDER BY Page", connection)).ExecuteReader(CommandBehavior.SingleResult);
          while (pageCountReader.Read())
          {
            pages++;
          }
          pageCountReader.Close();

          OleDbDataReader pageReader = (new OleDbCommand("SELECT DISTINCT Page FROM GraphicsUnits ORDER BY Page", connection)).ExecuteReader(CommandBehavior.SingleResult);

          Dictionary<String, PointF> pageOffset = new Dictionary<String,PointF>();

          int sqrtPages = (int)System.Math.Round(System.Math.Sqrt((double)pages)+0.5);
          int i = 0;
          int j = 0;
          float dX = 0.0F;
          float dY = 0.0F;
          while (pageReader.Read())
          {
            pageOffset.Add("/" + filename + "/" + pageReader.GetString(0) + "/", new PointF(dX, dY));

            i++;
            dX += 400.0F;
            if (i > sqrtPages-1)
            {
              i = 0;
              dX = 0.0F;
              j++;
              dY += 320.0F;
            }
          }
          pageReader.Close();

          foreach (GraphicItem graphicItem in graphic.graphicItems.Values)
          {
            graphicItem.X += pageOffset[graphicItem.Path].X;
            graphicItem.Y += pageOffset[graphicItem.Path].Y;
          }

          foreach (GraphicLink graphicLink in graphic.graphicLinks.Values)
          {
            GraphicItem sourceGraphicItem;
            graphic.graphicItems.TryGetValue(graphicLink.Origin, out sourceGraphicItem);

            GraphicItem destinationGraphicItem;
            graphic.graphicItems.TryGetValue(graphicLink.Origin, out destinationGraphicItem);

            if ((sourceGraphicItem != null) || (destinationGraphicItem != null))
            {
              List<PointF> controlPoints = new List<PointF>();
              foreach (PointF point in graphicLink.controlPoints)
              {
                controlPoints.Add(new PointF(point.X + pageOffset[sourceGraphicItem.Path].X, point.Y + pageOffset[sourceGraphicItem.Path].Y));
              }
              graphicLink.controlPoints = controlPoints;
            }
          }
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

using System;
using System.Collections.Generic;
using System.Text;
using MindFusion.FlowChartX;
using System.Drawing;
using SysCAD.Interface;

namespace SysCAD.Editor
{
  public class State
  {
    private Dictionary<Guid, Link> links = new Dictionary<Guid, Link>();
    private Dictionary<Guid, Item> items = new Dictionary<Guid, Item>();

    private ClientGraphic graphic;
    private Config config;

    private PureComponents.TreeView.TreeView tvNavigation;

    public bool ShowModels = false;
    public bool ShowGraphics = true;
    public bool ShowLinks = true;
    public bool ShowTags = true;

    public bool SelectLinks = true;
    public bool SelectItems = true;

    public IEnumerable<GraphicStencil> GraphicStencils
    {
      get { return config.graphicStencils.Values; }
    }

    public IEnumerable<ModelStencil> ModelStencils
    {
      get { return config.modelStencils.Values; }
    }

    public Config Config
    {
      set { config = value; }
    }

    public ClientGraphic Graphic
    {
      set { graphic = value; }
    }

    public PureComponents.TreeView.TreeView TvNavigation
    {
      set { tvNavigation = value; }
    }

    public Arrow Arrow(Guid guid)
    {
      Link link;
      links.TryGetValue(guid, out link);
      return link.Arrow;
    }

    public Item item(Guid guid)
    {
      Item item;
      items.TryGetValue(guid, out item);
      return item;
    }

    public void setArrow(Guid guid, String tag, Arrow arrow, GraphicLink graphicLink)
    {
      Link link = new Link(guid, tag, graphicLink);
      link.Arrow = arrow;
      links.Add(guid, link);
    }

    internal void Remove(FlowChart flowChart)
    {
      BoxCollection boxSelection = flowChart.Selection.Boxes.Clone();

      foreach (Box box in boxSelection)
      {
        Item item = box.Tag as Item;
        if (item != null)
        {
          item.Remove(flowChart);
          items.Remove(item.Guid);
        }
      }

      ArrowCollection arrowSelection = flowChart.Selection.Arrows.Clone();

      foreach (Arrow arrow in arrowSelection)
      {
        Link link = arrow.Tag as Link;
        if (link != null)
        {
          link.Remove(flowChart);
          links.Remove(link.Guid);
        }
      }
    }

    internal void CreateItem(GraphicItem graphicItem, bool isVisible, FlowChart flowchart)
    {
      flowchart.SuspendLayout();

      Box modelBox = flowchart.CreateBox(0.0F, 0.0F, 10.0F, 10.0F);
      Box graphicBox = flowchart.CreateBox(0.0F, 0.0F, 10.0F, 10.0F);
      Box textBox = flowchart.CreateBox(0.0F, 0.0F, 10.0F, 10.0F);

      modelBox.BoundingRect = new RectangleF(graphicItem.X, graphicItem.Y, graphicItem.Width, graphicItem.Height);
      modelBox.RotationAngle = graphicItem.Angle;
      modelBox.ToolTip = graphicItem.Tag + "\n\nClassID: " + graphicItem.Model;
      modelBox.Style = BoxStyle.Shape;
      {
        ModelStencil stencil;
        if (config.modelStencils.TryGetValue(graphicItem.Model, out stencil))
          modelBox.Shape = stencil.ShapeTemplate(graphicItem.MirrorX, graphicItem.MirrorY);
        else
          modelBox.Shape = ShapeTemplate.FromId("Decision2");

        AnchorPointCollection anchorPointCollection = new AnchorPointCollection();
        if (stencil.Anchors != null)
        {
          int anchorInt = 0;
          foreach (Anchor anchor in stencil.Anchors)
          {
            graphicItem.anchorIntToTag.Add(anchorInt, anchor.tag);
            graphicItem.anchorTagToInt.Add(anchor.tag, anchorInt);
            anchorInt++;
            AnchorPoint anchorPoint = new AnchorPoint((short)anchor.position.X, (short)anchor.position.Y, true, true, MarkStyle.Circle, Color.Green);
            anchorPoint.Tag = anchor;
            anchorPointCollection.Add(anchorPoint);
          }
          modelBox.AnchorPattern = new AnchorPattern(anchorPointCollection);
        }
      }

      modelBox.FillColor = Color.FromArgb(150, System.Drawing.Color.BurlyWood);
      modelBox.FrameColor = Color.FromArgb(200, System.Drawing.Color.BurlyWood);
      modelBox.Visible = ShowModels && isVisible;

      graphicBox.BoundingRect = new RectangleF(graphicItem.X, graphicItem.Y, graphicItem.Width, graphicItem.Height);
      graphicBox.AttachTo(modelBox, 0, 0, 100, 100);
      graphicBox.RotationAngle = graphicItem.Angle;
      graphicBox.ToolTip = graphicItem.Tag + "\n\nClassID: " + graphicItem.Model;
      graphicBox.Style = BoxStyle.Shape;
      {
        GraphicStencil stencil;
        if (config.graphicStencils.TryGetValue(graphicItem.Shape, out stencil))
          graphicBox.Shape = stencil.ShapeTemplate(graphicItem.MirrorX, graphicItem.MirrorY);
        else
          graphicBox.Shape = ShapeTemplate.FromId("Decision2");
      }
      graphicBox.EnabledHandles = Handles.None;
      graphicBox.HandlesStyle = HandlesStyle.Invisible;
      graphicBox.Visible = ShowGraphics && isVisible;


      if (graphicItem.FillColor.IsEmpty)
        graphicItem.FillColor = graphicBox.FillColor;
      else
        graphicBox.FillColor = graphicItem.FillColor;


      textBox.BoundingRect = new RectangleF(graphicItem.X, graphicItem.Y, graphicItem.Width, graphicItem.Height);
      textBox.AttachTo(modelBox, AttachToNode.BottomCenter);
      textBox.FillColor = Color.FromArgb(0, System.Drawing.Color.Black);
      textBox.FrameColor = Color.FromArgb(0, System.Drawing.Color.Black);
      textBox.Visible = ShowTags && isVisible;
      textBox.Text = graphicItem.Tag;


      Item item = new Item(graphicItem.Guid, graphicItem.Tag, modelBox, graphicBox, textBox, isVisible, graphicItem);


      modelBox.Tag = item;
      graphicBox.Tag = item;
      textBox.Tag = item;

      items.Add(item.Guid, item);
      flowchart.ResumeLayout();
    }

    internal bool DeleteItem(Guid guid)
    {
      //TBD: unlink connected links first
      return items.Remove(guid);
    }


    
    internal void CreateLink(GraphicLink graphicLink, bool isVisible, FlowChart flowchart)
    {
      Arrow arrow = flowchart.CreateArrow(new PointF(0.0F, 0.0F), new PointF(10.0F, 10.0F));

      Item origin = null;
      Item destination = null;

      if (graphicLink.Origin != null) origin = item(graphicLink.Origin);
      if (graphicLink.Destination != null) destination = item(graphicLink.Destination);

      PointF pointOrigin = new PointF();
      PointF pointDestination = new PointF();

      if (graphicLink.controlPoints != null && graphicLink.controlPoints.Count > 1)
      {
        pointOrigin = graphicLink.controlPoints[0];
        pointDestination = graphicLink.controlPoints[graphicLink.controlPoints.Count - 1];
      }

      if (origin != null)
        arrow.Origin = origin.Model;
      if (destination != null)
        arrow.Destination = destination.Model;



      if ((origin.Model.Tag as Item).GraphicItem.anchorTagToInt.ContainsKey(graphicLink.OriginPort))
        arrow.OrgnAnchor = (origin.Model.Tag as Item).GraphicItem.anchorTagToInt[graphicLink.OriginPort];
      else
        arrow.OrgnAnchor = -1;

      if ((destination.Model.Tag as Item).GraphicItem.anchorTagToInt.ContainsKey(graphicLink.DestinationPort))
        arrow.DestAnchor = (destination.Model.Tag as Item).GraphicItem.anchorTagToInt[graphicLink.DestinationPort];
      else
        arrow.DestAnchor = -1;


      arrow.ToolTip = graphicLink.Tag + "\n\nSrc: " + graphicLink.Origin + "\nDst: " + graphicLink.Destination;
      arrow.ArrowHead = ArrowHead.Triangle;
      arrow.Style = ArrowStyle.Cascading;

      if (graphicLink.controlPoints != null && graphicLink.controlPoints.Count > 1)
      {
        arrow.SegmentCount = (short)(graphicLink.controlPoints.Count - 1);
        int i = 0;
        foreach (PointF point in graphicLink.controlPoints)
          arrow.ControlPoints[i++] = point;
        arrow.UpdateFromPoints();
        PointCollection a = arrow.ControlPoints;
      }

      Link link = new Link(graphicLink.Guid, graphicLink.Tag, graphicLink);
      link.Arrow = arrow;
      link.Visible = true;

      arrow.Tag = link;

      arrow.Visible = ShowLinks && isVisible;

      links.Add(link.Guid, link);
    }

    internal bool DeleteLink(Guid guid)
    {
      return links.Remove(guid);
    }



    internal void ItemVisible(Guid guid, bool visible)
    {
      Item item;
      if (items.TryGetValue(guid, out item))
      {
        item.Visible = visible;
        item.Model.Visible = visible && (item.Model.Selected || ShowModels);
        item.Graphic.Visible = visible && ShowGraphics;
        item.Text.Visible = visible && ShowTags;

        foreach (Arrow arrowDestination in item.Model.IncomingArrows)
        {
          arrowDestination.Visible = visible && ShowLinks;
        }

        foreach (Arrow arrowOrigin in item.Model.OutgoingArrows)
        {
          arrowOrigin.Visible = visible && ShowLinks;
        }
      }
    }

    internal void ItemSelected(Guid guid, bool selected)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        item.Selected = selected;
        item.Model.Selected = selected && item.Visible;
      }
    }

    internal bool Exists(Guid guid)
    {
      Item item;
      items.TryGetValue(guid, out item);

      Link link;
      links.TryGetValue(guid, out link);

      return ((link != null) || (item != null));
    }

    internal bool Exists(String tag)
    {
      foreach (Item item in items.Values)
      {
        if (item.Tag == tag)
          return true;
      }

      foreach (Link link in links.Values)
      {
        if (link.Tag == tag)
          return true;
      }

      return false;
    }

    internal void SetAngle(Guid guid, float angle)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        item.Model.RotationAngle = angle;
        item.Graphic.RotationAngle = angle;
      }
    }

    internal void SetHeight(Guid guid, float height)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        RectangleF boundingRect = item.Model.BoundingRect;
        boundingRect.Height = height;
        item.Model.BoundingRect = boundingRect;
        item.Graphic.BoundingRect = boundingRect;
      }
    }

    internal void SetWidth(Guid guid, float width)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        RectangleF boundingRect = item.Model.BoundingRect;
        boundingRect.Width = width;
        item.Model.BoundingRect = boundingRect;
        item.Graphic.BoundingRect = boundingRect;
      }
    }

    internal void SetX(Guid guid, float x)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        RectangleF boundingRect = item.Model.BoundingRect;
        boundingRect.X = x;
        item.Model.BoundingRect = boundingRect;
        item.Graphic.BoundingRect = boundingRect;
      }
    }

    internal void SetY(Guid guid, float y)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        RectangleF boundingRect = item.Model.BoundingRect;
        boundingRect.Y = y;
        item.Model.BoundingRect = boundingRect;
        item.Graphic.BoundingRect = boundingRect;
      }
    }

    internal void SetMirrorX(Guid guid, bool mirrorX)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        GraphicItem graphicItem;
        if (graphic.graphicItems.TryGetValue(guid, out graphicItem))
        {
          GraphicStencil stencil;
          if (config.graphicStencils.TryGetValue(graphicItem.Shape, out stencil))
          {
            item.Graphic.Shape = stencil.ShapeTemplate(graphicItem.MirrorX, graphicItem.MirrorY);
          }
        }
      }
    }

    internal void SetMirrorY(Guid guid, bool mirrorY)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        GraphicItem graphicItem;
        if (graphic.graphicItems.TryGetValue(guid, out graphicItem))
        {
          GraphicStencil stencil;
          if (config.graphicStencils.TryGetValue(graphicItem.Shape, out stencil))
          {
            item.Graphic.Shape = stencil.ShapeTemplate(graphicItem.MirrorX, graphicItem.MirrorY);
          }
        }
      }
    }

    internal void SetTag(Guid guid, String tag)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        item.Tag = tag;
        item.Text.Text = tag;
      }
    }

    internal void SetFillColor(Guid guid, Color fillColor)
    {
      Item item;
      items.TryGetValue(guid, out item);
      if (item != null)
      {
        item.Graphic.FillColor = fillColor;
      }
    }

    internal ModelStencil ModelShape(string stencilName)
    {
      ModelStencil modelStencil;
      config.modelStencils.TryGetValue(stencilName, out modelStencil);
      return modelStencil;
    }

    internal GraphicStencil GraphicShape(string stencilName)
    {
      GraphicStencil graphicStencil;
      config.graphicStencils.TryGetValue(stencilName, out graphicStencil);
      return graphicStencil;
    }

    internal GraphicItem GraphicItem(Guid guid)
    {
      GraphicItem graphicItem;
      graphic.graphicItems.TryGetValue(guid, out graphicItem);
      return graphicItem;
    }

    internal GraphicItem GraphicItem(Box box)
    {
      GraphicItem graphicItem = null;
      if (box.Tag != null)
        graphic.graphicItems.TryGetValue((box.Tag as Item).Guid, out graphicItem);
      return graphicItem;
    }

    internal GraphicLink GraphicLink(Guid guid)
    {
      GraphicLink graphicLink;
      graphic.graphicLinks.TryGetValue(guid, out graphicLink);
      return graphicLink;
    }

    internal GraphicLink GraphicLink(Arrow arrow)
    {
      GraphicLink graphicLink;
      graphic.graphicLinks.TryGetValue((arrow.Tag as Link).Guid, out graphicLink);
      return graphicLink;
    }

    internal IEnumerable<GraphicItem> GraphicItems
    {
      get { return graphic.graphicItems.Values; }
    }

    internal IEnumerable<GraphicLink> GraphicLinks
    {
      get { return graphic.graphicLinks.Values; }
    }

    internal Item Item(Guid guid)
    {
      Item item;
      items.TryGetValue(guid, out item);
      return item;
    }

    internal Link Link(Guid guid)
    {
      Link link;
      links.TryGetValue(guid, out link);
      return link;
    }

    internal IEnumerable<Item> Items
    {
      get { return items.Values; }
    }

    internal IEnumerable<Link> Links
    {
      get { return links.Values; }
    }

    internal string CurrentPath
    {
      get { return tvNavigation.SelectedNode.FullPath; }
    }


    internal bool IsItem(Guid guid)
    {
      return graphic.graphicItems.ContainsKey(guid);
    }



    internal bool ModifyGraphicItem(out uint requestID, Guid guid, String tag, String path, String model, String shape, RectangleF boundingRect, Single angle, System.Drawing.Color fillColor, bool mirrorX, bool mirrorY)
    {
      return graphic.ModifyItem(out requestID, guid, tag, path, model, shape, boundingRect, angle, fillColor, mirrorX, mirrorY);
    }

    internal bool CreateGraphicItem(out uint requestID, out Guid guid, String tag, String path, String model, String shape, RectangleF boundingRect, Single angle, Color fillColor, bool mirrorX, bool mirrorY)
    {
      return graphic.CreateItem(out requestID, out guid, tag, path, model, shape, boundingRect, angle, fillColor, mirrorX, mirrorY);
    }

    internal bool DeleteGraphicItem(out uint requestID, Guid guid)
    {
      return graphic.DeleteItem(out requestID, guid);
    }



    internal bool ModifyGraphicLink(out uint requestID, Guid guid, String tag, String classID, Guid origin, Guid destination, String originPort, String destinationPort, List<PointF> controlPoints)
    {
      return graphic.ModifyLink(out requestID, guid, tag, classID, origin, destination, originPort, destinationPort, controlPoints);
    }

    internal bool CreateGraphicLink(out uint requestID, Guid guid, String tag, String classID, Guid origin, Guid destination, String originPort, String destinationPort, List<PointF> controlPoints)
    {
      return graphic.CreateLink(out requestID, guid, tag, classID, origin, destination, originPort, destinationPort, controlPoints);
    }

    internal bool DeleteGraphicLink(out uint requestID, Guid guid)
    {
      return graphic.DeleteLink(out requestID, guid);
    }


    internal PortStatus PortCheck(Guid itemGuid, Anchor anchor, Guid linkGuid)
    {
      return graphic.PortCheck(itemGuid, anchor, linkGuid);
    }


    internal void ConnectGraphic(
      ClientGraphic.ItemCreatedHandler itemCreatedHandler,
      ClientGraphic.ItemModifiedHandler itemModifiedHandler,
      ClientGraphic.ItemDeletedHandler itemDeletedHandler,
      ClientGraphic.LinkCreatedHandler LinkCreatedHandler,
      ClientGraphic.LinkModifiedHandler LinkModifiedHandler,
      ClientGraphic.LinkDeletedHandler LinkDeletedHandler)
    {
      graphic.ItemCreated += itemCreatedHandler;
      graphic.ItemModified += itemModifiedHandler;
      graphic.ItemDeleted += itemDeletedHandler;

      graphic.LinkCreated += LinkCreatedHandler;
      graphic.LinkModified += LinkModifiedHandler;
      graphic.LinkDeleted += LinkDeletedHandler;
    }


    internal void DisconnectGraphic(
      ClientGraphic.ItemCreatedHandler itemCreatedHandler,
      ClientGraphic.ItemModifiedHandler itemModifiedHandler,
      ClientGraphic.ItemDeletedHandler itemDeletedHandler,
      ClientGraphic.LinkCreatedHandler LinkCreatedHandler,
      ClientGraphic.LinkModifiedHandler LinkModifiedHandler,
      ClientGraphic.LinkDeletedHandler LinkDeletedHandler)
    {
      graphic.ItemCreated -= itemCreatedHandler;
      graphic.ItemModified -= itemModifiedHandler;
      graphic.ItemDeleted -= itemDeletedHandler;

      graphic.LinkCreated -= LinkCreatedHandler;
      graphic.LinkModified -= LinkModifiedHandler;
      graphic.LinkDeleted -= LinkDeletedHandler;
    }



    internal void AddNode(string path, string tag, Guid guid)
    {
      tvNavigation.GetNodeByPath(path).Nodes.Add(tag, guid.ToString());
    }
  }
}
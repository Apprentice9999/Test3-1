using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;

using MindFusion.FlowChartX;

using SysCAD.Interface;
using PureComponents.TreeView;
using System.Windows.Forms;
using ActiproSoftware.UIStudio.Bar;

namespace SysCAD.Editor
{
  public partial class FrmFlowChart : Form
  {
    public State state = new State();

    public string currentModelShape;
    public string currentGraphicShape;

    public Arrow arrowBeingModified;

    private int tempBoxKey = 0;
    private int tempArrowKey = 0;

    private Form1 form1;

    public FrmFlowChart(Form1 form1)
    {
      this.form1 = form1;

      InitializeComponent();

      fcFlowChart.DocExtents = new RectangleF(0.0F, 0.0F, 297.0F, 210.0F);
      fcFlowChart.Selection.Style = SelectionStyle.SemiTransparent;
    }

    ~FrmFlowChart()
    {
      state.DisconnectGraphic(
        new ClientGraphic.ItemCreatedHandler(fcFlowChart_ItemCreated),
        new ClientGraphic.ItemModifiedHandler(fcFlowChart_ItemModified),
        new ClientGraphic.ItemDeletedHandler(fcFlowChart_ItemDeleted),
        new ClientGraphic.LinkCreatedHandler(fcFlowChart_LinkCreated),
        new ClientGraphic.LinkModifiedHandler(fcFlowChart_LinkModified),
        new ClientGraphic.LinkDeletedHandler(fcFlowChart_LinkDeleted));
    }

    internal void SetProject(ClientGraphic graphic, Config config, PureComponents.TreeView.TreeView tvNavigation)
    {
      state.Graphic = graphic;
      state.Config = config;
      state.TvNavigation = tvNavigation;

      state.ConnectGraphic(
        new ClientGraphic.ItemCreatedHandler(fcFlowChart_ItemCreated), 
        new ClientGraphic.ItemModifiedHandler(fcFlowChart_ItemModified),
        new ClientGraphic.ItemDeletedHandler(fcFlowChart_ItemDeleted),
        new ClientGraphic.LinkCreatedHandler(fcFlowChart_LinkCreated),
        new ClientGraphic.LinkModifiedHandler(fcFlowChart_LinkModified),
        new ClientGraphic.LinkDeletedHandler(fcFlowChart_LinkDeleted));

      fcFlowChart.UndoManager.UndoEnabled = false;
      fcFlowChart.UseWaitCursor = true;
      fcFlowChart.ValidityChecks = false;
      fcFlowChart.Visible = false;
      fcFlowChart.Enabled = false;

      foreach (GraphicItem graphicItem in graphic.graphicItems.Values)
      {
        state.CreateItem(graphicItem, false, fcFlowChart);
      }

      foreach (GraphicLink graphicLink in graphic.graphicLinks.Values)
      {
        if (graphicLink.ClassID == "Pipe-1")
        {
          state.CreateLink(graphicLink, false, fcFlowChart);
        }
      }

      fcFlowChart.UndoManager.UndoEnabled = true;
      fcFlowChart.UseWaitCursor = false;
      fcFlowChart.ValidityChecks = true;
      fcFlowChart.Visible = true;
      fcFlowChart.Enabled = true;
    }

    internal void UnsetProject()
    {

    }

    public void ZoomToVisible()
    {
      FixDocExtents();

      float minX =  Single.MaxValue, minY =  Single.MaxValue;
      float maxX = -Single.MaxValue, maxY = -Single.MaxValue;

      bool foundVisibleObject = false;

      foreach (Box box in fcFlowChart.Boxes)
      {
        if (box.Visible)
        {
          foundVisibleObject = true;
          if (box.BoundingRect.Left < minX)   minX = box.BoundingRect.Left;
          if (box.BoundingRect.Top < minY)    minY = box.BoundingRect.Top;
          if (box.BoundingRect.Right > maxX)  maxX = box.BoundingRect.Right;
          if (box.BoundingRect.Bottom > maxY) maxY = box.BoundingRect.Bottom;
        }
      }

      foreach (Arrow arrow in fcFlowChart.Arrows)
      {
        if (arrow.Visible)
        {
          foundVisibleObject = true;
          if (arrow.BoundingRect.Left < minX)   minX = arrow.BoundingRect.Left;
          if (arrow.BoundingRect.Top < minY)    minY = arrow.BoundingRect.Top;
          if (arrow.BoundingRect.Right > maxX)  maxX = arrow.BoundingRect.Right;
          if (arrow.BoundingRect.Bottom > maxY) maxY = arrow.BoundingRect.Bottom;
        }
      }

      float width = maxX - minX;
      float height = maxY - minY;

      if (foundVisibleObject)
        fcFlowChart.ZoomToRect(RectangleF.FromLTRB(minX - width * 0.05F, minY - height * 0.05F, maxX + width * 0.05F, maxY + height * 0.05F));
      else
        fcFlowChart.ZoomToRect(fcFlowChart.DocExtents);

      fcFlowChart.Invalidate();
    }

    public void FixDocExtents()
    {
      float minX = Single.MaxValue, minY = Single.MaxValue;
      float maxX = -Single.MaxValue, maxY = -Single.MaxValue;

      bool foundObject = false;

      foreach (Box box in fcFlowChart.Boxes)
      {
        foundObject = true;
        if (box.BoundingRect.Left < minX) minX = box.BoundingRect.Left;
        if (box.BoundingRect.Top < minY) minY = box.BoundingRect.Top;
        if (box.BoundingRect.Right > maxX) maxX = box.BoundingRect.Right;
        if (box.BoundingRect.Bottom > maxY) maxY = box.BoundingRect.Bottom;
      }

      foreach (Arrow arrow in fcFlowChart.Arrows)
      {
        foundObject = true;
        if (arrow.BoundingRect.Left < minX) minX = arrow.BoundingRect.Left;
        if (arrow.BoundingRect.Top < minY) minY = arrow.BoundingRect.Top;
        if (arrow.BoundingRect.Right > maxX) maxX = arrow.BoundingRect.Right;
        if (arrow.BoundingRect.Bottom > maxY) maxY = arrow.BoundingRect.Bottom;
      }

      float width = maxX - minX;
      float height = maxY - minY;

      if (foundObject)
        fcFlowChart.DocExtents = RectangleF.FromLTRB(minX - width * 0.05F, minY - height * 0.05F, maxX + width * 0.05F, maxY + height * 0.05F);
    }

    public void ZoomToSelected()
    {
      FixDocExtents();

      float minX = Single.MaxValue, minY = Single.MaxValue;
      float maxX = -Single.MaxValue, maxY = -Single.MaxValue;

      bool foundSelectedObject = false;

      foreach (Box box in fcFlowChart.Boxes)
      {
        if (box.Selected)
        {
          foundSelectedObject = true;
          if (box.BoundingRect.Left < minX)   minX = box.BoundingRect.Left;
          if (box.BoundingRect.Top < minY)    minY = box.BoundingRect.Top;
          if (box.BoundingRect.Right > maxX)  maxX = box.BoundingRect.Right;
          if (box.BoundingRect.Bottom > maxY) maxY = box.BoundingRect.Bottom;
        }
      }

      foreach (Arrow arrow in fcFlowChart.Arrows)
      {
        if (arrow.Selected)
        {
          foundSelectedObject = true;
          if (arrow.BoundingRect.Left < minX)   minX = arrow.BoundingRect.Left;
          if (arrow.BoundingRect.Top < minY)    minY = arrow.BoundingRect.Top;
          if (arrow.BoundingRect.Right > maxX)  maxX = arrow.BoundingRect.Right;
          if (arrow.BoundingRect.Bottom > maxY) maxY = arrow.BoundingRect.Bottom;
        }
      }

      float width = maxX - minX;
      float height = maxY - minY;

      if (foundSelectedObject)
        fcFlowChart.ZoomToRect(RectangleF.FromLTRB(minX - width * 0.05F, minY - height * 0.05F, maxX + width * 0.05F, maxY + height * 0.05F));
      else
        fcFlowChart.ZoomToRect(fcFlowChart.DocExtents);
    }

    private void fcFlowChart_ItemCreated(uint eventID, uint requestID, Guid guid, String tag, String path, String model, String shape, RectangleF boundingRect, Single angle, System.Drawing.Color fillColor, bool mirrorX, bool mirrorY)
    {
      state.AddNode(path, tag, guid);
      state.CreateItem(state.GraphicItem(guid), true, fcFlowChart);
    }

    private void fcFlowChart_ItemModified(uint eventID, uint requestID, Guid guid, String tag, String path, String model, String shape, RectangleF boundingRect, Single angle, System.Drawing.Color fillColor, bool mirrorX, bool mirrorY)
    {
      Item item = state.Item(guid);
      if (item != null)
      {
        item.Model.BoundingRect = boundingRect;
        item.Graphic.BoundingRect = boundingRect;
        item.Model.RotationAngle = angle;
        item.Graphic.RotationAngle = angle;
      }
    }

    private void fcFlowChart_ItemDeleted(uint eventID, uint requestID, Guid guid)
    {
      state.DeleteItem(guid);
    }



    private void fcFlowChart_LinkCreated(uint eventID, uint requestID, Guid guid, String tag, String classID, Guid origin, Guid destination, String originPort, String destinationPort, List<PointF> controlPoints)
    {
      state.CreateLink(state.GraphicLink(guid), true, fcFlowChart);
    }

    private void fcFlowChart_LinkModified(uint eventID, uint requestID, Guid guid, String tag, String classID, Guid origin, Guid destination, String originPort, String destinationPort, List<PointF> controlPoints)
    {
      Link Link = state.Link(guid);
      if (Link != null)
      {
        //Link.Arrow.
        //Link.Model.BoundingRect = boundingRect;
        //Link.Graphic.BoundingRect = boundingRect;
        //Link.Model.RotationAngle = angle;
        //Link.Graphic.RotationAngle = angle;
      }
    }

    private void fcFlowChart_LinkDeleted(uint eventID, uint requestID, Guid guid)
    {
      // TBD
    }



    private void fcFlowChart_ArrowAttaching(object sender, AttachConfirmArgs e)
    {
      if (e.ChangingOrigin) // Origin is trying to be being changed.
      {
        if (e.Origin==null)
          e.Confirm = false; // Trying to be disconnected, ignore.
        else
          e.Confirm = true; // Being attached to another node, OK.
      }
      else // Destination is trying to be being changed.
      {
        if (e.Destination==null)
          e.Confirm = false; // Trying to be disconnected, ignore.
        else
          e.Confirm = true; // Being attached to another node, OK.
      }
    }

    private void fcFlowChart_ArrowModified(object sender, ArrowMouseArgs e)
    {
      arrowBeingModified = null;
      if (fcFlowChart.Selection.Arrows.Count == 1) // We're playing with just one arrow...
      {
        PointCollection controlPoints = e.Arrow.ControlPoints.Clone();

        if (e.Arrow.Origin is Box)
        {
          Item originItem = (e.Arrow.Origin.Tag as Item);
          if (originItem != null)
          {
            Box originBox = originItem.Model;
            (e.Arrow.Tag as Link).graphicLink.Origin = originItem.Guid;
            e.Arrow.Origin = originBox;
            e.Arrow.OrgnAnchor = -1;
            for (int i = 0; i < originBox.AnchorPattern.Points.Count; i++)
            {
              if (originBox.AnchorPattern.Points[i].AllowOutgoing)
              {
                e.Arrow.OrgnAnchor = i;
              }
            }
          }
        }
        else
        {
          Guid originGuid = (e.Arrow.Tag as Link).graphicLink.Origin;
          Item originItem = state.Item(originGuid);

          if (originItem != null)
          {
            Box originBox = originItem.Model;
            e.Arrow.Origin = originBox;
            e.Arrow.OrgnAnchor = -1;
            for (int i = 0; i < originBox.AnchorPattern.Points.Count; i++)
            {
              if (originBox.AnchorPattern.Points[i].AllowOutgoing)
              {
                e.Arrow.OrgnAnchor = i;
              }
            }
          }
        }

        if (e.Arrow.Destination is Box)
        {
          Item destinationItem = (e.Arrow.Destination.Tag as Item);
          if (destinationItem != null)
          {
            Box destinationBox = destinationItem.Model;
            (e.Arrow.Tag as Link).graphicLink.Destination = destinationItem.Guid;
            e.Arrow.Destination = destinationItem.Model;
            e.Arrow.DestAnchor = -1;
            for (int i=0; i<destinationBox.AnchorPattern.Points.Count; i++)
            {
              if (destinationBox.AnchorPattern.Points[i].AllowIncoming)
              {
                e.Arrow.DestAnchor = i;
              }
            }
          }

        }
        else
        {
          Guid destinationGuid = (e.Arrow.Tag as Link).graphicLink.Destination;
          Item destinationItem = state.Item(destinationGuid);

          if (destinationItem != null)
          {
            Box destinationBox = destinationItem.Model;
            e.Arrow.Destination = destinationBox;
            e.Arrow.DestAnchor = -1;
            for (int i = 0; i < destinationBox.AnchorPattern.Points.Count; i++)
            {
              if (destinationBox.AnchorPattern.Points[i].AllowIncoming)
              {
                e.Arrow.DestAnchor = i;
              }
            }
          }
        }

        e.Arrow.ControlPoints.Clear();
        foreach (PointF point in controlPoints)
        {
          e.Arrow.ControlPoints.Add(point);
        }
        e.Arrow.UpdateFromPoints();
      }
    }

    private void fcFlowChart_ArrowModifying(object sender, ArrowMouseArgs e)
    {
      if (fcFlowChart.Selection.Arrows.Count == 1) // We're playing with just one arrow...
      {
        arrowBeingModified = e.Arrow;
      }
      fcFlowChart.Invalidate();
    }

    private Item oldHoverItem = null;
    private Arrow oldHoverArrow = null;

    public void fcFlowChart_MouseMove(object sender, MouseEventArgs e)
    {
      if (e.Button == MouseButtons.Left)
      {
        overBox = fcFlowChart.GetBoxAt(fcFlowChart.ClientToDoc(e.Location), 2);
        overArrow = fcFlowChart.GetArrowAt(fcFlowChart.ClientToDoc(e.Location), 2);

        if ((overBox != null) || (overArrow != null)) // we're not in free space...
        {
          if ((form1.barManager1.Commands["Mode.CreateNode"] as BarButtonCommand).Checked == true)
            form1.Mode_Modify();
        }
      }

      Box hoverBox = fcFlowChart.GetBoxAt(fcFlowChart.ClientToDoc(new System.Drawing.Point(e.X, e.Y)), 2);
      Item hoverItem = null;
      if (hoverBox != null)
        hoverItem = hoverBox.Tag as Item;

      Arrow hoverArrow = fcFlowChart.GetArrowAt(fcFlowChart.ClientToDoc(new System.Drawing.Point(e.X, e.Y)), 1);


      if (oldHoverItem != null) // deal with old itemBox.
      {
        if (oldHoverItem == hoverItem) // nothings changed.
          return;
        else // we've moved on, un-hover the old one.
        {
          oldHoverItem.Graphic.Visible = oldHoverItem.Visible && state.ShowGraphics;
          //oldHoverItem.Graphic.ZTop();
          oldHoverItem.Model.Visible = oldHoverItem.Visible && (oldHoverItem.Model.Selected || state.ShowModels);
          oldHoverItem.Model.ZIndex = oldHoverItem.Graphic.ZIndex + 1;
          oldHoverItem.Text.Visible = oldHoverItem.Visible && state.ShowTags;
          oldHoverItem.Text.ZIndex = oldHoverItem.Graphic.ZIndex + 2;

          foreach (Arrow arrow in oldHoverItem.IncomingArrows)
          {
            arrow.Visible = oldHoverItem.Visible && state.ShowLinks;
            arrow.ZIndex = oldHoverItem.Graphic.ZIndex + 1;
          }

          foreach (Arrow arrow in oldHoverItem.OutgoingArrows)
          {
            arrow.Visible = oldHoverItem.Visible && state.ShowLinks;
            arrow.ZIndex = oldHoverItem.Graphic.ZIndex + 1;
          }
        }
      }

      if (oldHoverArrow != null) // deal with old arrow.
      {
        if (oldHoverArrow == hoverArrow) // nothings changed...
          return;
        else
        {
          Box originBox = (oldHoverArrow.Origin as Box);
          if (originBox != null)
          {
            Item originItem = originBox.Tag as Item;
            if (originItem != null)
            {
              originItem.Graphic.Visible = originItem.Visible && state.ShowGraphics;
              //originItem.Graphic.ZBottom();
              originItem.Model.Visible = originItem.Visible && (originItem.Model.Selected || state.ShowModels);
              originItem.Model.ZIndex = originItem.Graphic.ZIndex + 1;
              originItem.Text.Visible = originItem.Visible && state.ShowTags;
              originItem.Text.ZIndex = originItem.Graphic.ZIndex + 2;
            }
          }

          Box destinationBox = (oldHoverArrow.Destination as Box);
          if (destinationBox != null)
          {
            Item destinationItem = destinationBox.Tag as Item;
            if (destinationItem != null)
            {
              destinationItem.Graphic.Visible = destinationItem.Visible && state.ShowGraphics;
              //destinationItem.Graphic.ZBottom();
              destinationItem.Model.Visible = destinationItem.Visible && (destinationItem.Model.Selected || state.ShowModels);
              destinationItem.Model.ZIndex = destinationItem.Graphic.ZIndex + 1;
              destinationItem.Text.Visible = destinationItem.Visible && state.ShowTags;
              destinationItem.Text.ZIndex = destinationItem.Graphic.ZIndex + 2;
            }
          }
        }

        oldHoverArrow.Visible = state.ShowLinks;
        //oldHoverArrow.ZTop();
      }

      if (hoverItem != null)
      {
        hoverItem.Graphic.Visible = hoverItem.Visible;
        //hoverItem.Graphic.ZBottom();
        hoverItem.Model.Visible = hoverItem.Visible;
        hoverItem.Model.ZIndex = hoverItem.Graphic.ZIndex + 1;
        hoverItem.Text.Visible = hoverItem.Visible && state.ShowTags;
        hoverItem.Text.ZIndex = hoverItem.Graphic.ZIndex + 2;

        foreach (Arrow arrow in hoverItem.IncomingArrows)
        {
          arrow.Visible = hoverItem.Visible;
          arrow.ZTop();
        }

        foreach (Arrow arrow in hoverItem.OutgoingArrows)
        {
          arrow.Visible = hoverItem.Visible;
          arrow.ZIndex = hoverItem.Graphic.ZIndex + 1;
        }
      }
      else if (hoverArrow != null)
      {
        Box originBox = (hoverArrow.Origin as Box);
        if (originBox != null)
        {
          Item originItem = originBox.Tag as Item;
          if (originItem != null)
          {
            originItem.Graphic.Visible = originItem.Visible;
            //originItem.Graphic.ZBottom();
            originItem.Model.Visible = originItem.Visible;
            originItem.Model.ZIndex = originItem.Graphic.ZIndex + 1;
            originItem.Text.Visible = originItem.Visible && state.ShowTags;
            originItem.Text.ZIndex = originItem.Graphic.ZIndex + 2;
          }
        }

        Box destinationBox = (hoverArrow.Destination as Box);
        if (destinationBox != null)
        {
          Item destinationItem = destinationBox.Tag as Item;
          if (destinationItem != null)
          {
            destinationItem.Graphic.Visible = destinationItem.Visible;
            //destinationItem.Graphic.ZBottom();
            destinationItem.Model.Visible = destinationItem.Visible;
            destinationItem.Model.ZIndex = destinationItem.Graphic.ZIndex + 1;
            destinationItem.Text.Visible = destinationItem.Visible && state.ShowTags;
            destinationItem.Text.ZIndex = destinationItem.Graphic.ZIndex + 2;
          }
        }

        hoverArrow.Visible = true;
        //hoverArrow.ZIndex = hoverItem.Graphic.ZIndex + 1;
      }

      oldHoverItem = hoverItem;
      oldHoverArrow = hoverArrow;

      //ResumeLayout(false);
    }

    private void fcFlowChart_DrawBox(object sender, BoxDrawArgs e)
    {
      PointF mousePos = fcFlowChart.ClientToDoc(MousePosition);

      foreach (Arrow arrow in e.Box.IncomingArrows)
      {
        if (arrow != arrowBeingModified)
        {
          float dx = 0.5F;
          float dy = 0.5F;

          if (arrow.DestAnchor != -1)
          {

            dx = e.Box.AnchorPattern.Points[arrow.DestAnchor].X / 100.0F;
            dy = e.Box.AnchorPattern.Points[arrow.DestAnchor].Y / 100.0F;
          }

          PointF anchorPointPos = new PointF(
            e.Box.BoundingRect.Left + e.Box.BoundingRect.Width * dx,
            e.Box.BoundingRect.Top + e.Box.BoundingRect.Height * dy);

          PointF[] extensionPoints =
            new PointF[] { arrow.ControlPoints[arrow.ControlPoints.Count - 1], anchorPointPos };

          System.Drawing.Pen pen = new System.Drawing.Pen(Color.Blue, 0.0F);

          e.Graphics.DrawLines(pen, extensionPoints);
        }
      }

      foreach (Arrow arrow in e.Box.OutgoingArrows)
      {
        if (arrow != arrowBeingModified)
        {
          float dx = 0.5F;
          float dy = 0.5F;

          if (arrow.OrgnAnchor != -1)
          {
            dx = e.Box.AnchorPattern.Points[arrow.OrgnAnchor].X / 100.0F;
            dy = e.Box.AnchorPattern.Points[arrow.OrgnAnchor].Y / 100.0F;
          }

          PointF anchorPointPos = new PointF(
            e.Box.BoundingRect.Left + e.Box.BoundingRect.Width * dx,
            e.Box.BoundingRect.Top + e.Box.BoundingRect.Height * dy);

          PointF[] extensionPoints =
            new PointF[] { arrow.ControlPoints[0], anchorPointPos };

          System.Drawing.Pen pen = new System.Drawing.Pen(Color.Blue, 0.0F);

          e.Graphics.DrawLines(pen, extensionPoints);
        }
      }

      if (arrowBeingModified != null)
      {
        PointF originPos = arrowBeingModified.ControlPoints[0];
        Box originBox = fcFlowChart.GetBoxAt(originPos, 2);
        if (originBox != null)
        {
          originBox = (originBox.Tag as Item).Model;
          if (originBox != null)
          {
            if (originBox.AnchorPattern != null)
            {
              foreach (AnchorPoint anchorPoint in originBox.AnchorPattern.Points)
              {
                if (anchorPoint.AllowOutgoing)
                {
                  float dx = anchorPoint.X / 100.0F;
                  float dy = anchorPoint.Y / 100.0F;
                  PointF anchorPointPos = new PointF(
                  originBox.BoundingRect.Left + originBox.BoundingRect.Width * dx,
                  originBox.BoundingRect.Top + originBox.BoundingRect.Height * dy);

                  PointF[] extensionPoints =
                    new PointF[] { anchorPointPos, anchorPointPos };
                  System.Drawing.Pen pen = new System.Drawing.Pen(Color.Yellow, 5.0F);
                  e.Graphics.DrawEllipse(pen, RectangleF.FromLTRB(
                    anchorPointPos.X - 1.0F,
                    anchorPointPos.Y - 1.0F,
                    anchorPointPos.X + 1.0F,
                    anchorPointPos.Y + 1.0F));
                  e.Graphics.DrawLines(pen, extensionPoints);
                }
              }
            }
          }
        }

        PointF destinationPos = arrowBeingModified.ControlPoints[arrowBeingModified.ControlPoints.Count-1];
        Box destinationBox = fcFlowChart.GetBoxAt(destinationPos, 2);
        if (destinationBox != null)
        {
          destinationBox = (destinationBox.Tag as Item).Model;
          if (destinationBox != null)
          {
            if (destinationBox.AnchorPattern != null)
            {
              foreach (AnchorPoint anchorPoint in destinationBox.AnchorPattern.Points)
              {
                if (anchorPoint.AllowIncoming)
                {
                  float dx = anchorPoint.X / 100.0F;
                  float dy = anchorPoint.Y / 100.0F;
                  PointF anchorPointPos = new PointF(
                  destinationBox.BoundingRect.Left + destinationBox.BoundingRect.Width * dx,
                  destinationBox.BoundingRect.Top + destinationBox.BoundingRect.Height * dy);

                  PointF[] extensionPoints =
                    new PointF[] { anchorPointPos, anchorPointPos };
                  System.Drawing.Pen pen = new System.Drawing.Pen(Color.Yellow, 5.0F);
                  e.Graphics.DrawEllipse(pen, RectangleF.FromLTRB(
                    anchorPointPos.X - 1.0F,
                    anchorPointPos.Y - 1.0F,
                    anchorPointPos.X + 1.0F,
                    anchorPointPos.Y + 1.0F));
                  e.Graphics.DrawLines(pen, extensionPoints);
                }
              }
            }
          }
        }
      }
    }

    private void fcFlowChart_DrawArrow(object sender, ArrowDrawArgs e)
    {

    }

    private void fcFlowChart_BoxModified(object sender, BoxMouseArgs e)
    {
      GraphicItem graphicItem = state.GraphicItem((e.Box.Tag as Item).Guid);
      Box modelBox = (e.Box.Tag as Item).Model;
      //Box textBox = (e.Box.Tag as Item).Text;

      //int x1 = (int)((textBox.BoundingRect.Left - modelBox.BoundingRect.Left) / modelBox.BoundingRect.Width*100.0);
      //int y1 = (int)((textBox.BoundingRect.Top - modelBox.BoundingRect.Top) / modelBox.BoundingRect.Height * 100.0);
      //int x2 = (int)((textBox.BoundingRect.Right - modelBox.BoundingRect.Left) / modelBox.BoundingRect.Width * 100.0);
      //int y2 = (int)((textBox.BoundingRect.Bottom - modelBox.BoundingRect.Top) / modelBox.BoundingRect.Height * 100.0);
      //textBox.AttachTo(modelBox, x1, y1, x2, y2);

      // calculate new boundingbox...
      uint requestID;
      if (!state.ModifyGraphicItem(out requestID,
        graphicItem.Guid,
        graphicItem.Tag,
        graphicItem.Path,
        graphicItem.Model,
        graphicItem.Shape,
        modelBox.BoundingRect,
        modelBox.RotationAngle,
        graphicItem.FillColor,
        graphicItem.MirrorX,
        graphicItem.MirrorY))
      { // failure, revert back to previous.
        modelBox.BoundingRect = graphicItem.BoundingRect;
        modelBox.RotationAngle = graphicItem.Angle;
      }

      form1.propertyGrid1.Refresh();

    }

    private void fcFlowChart_BoxModifying(object sender, BoxMouseArgs e)
    {
      Box graphicBox = (e.Box.Tag as Item).Graphic;
      //graphicBox.BoundingRect = (e.Box.Tag as Item).Model.BoundingRect;
      graphicBox.RotationAngle = (e.Box.Tag as Item).Model.RotationAngle;
    }

    public void NewGraphicItem(GraphicItem graphicItem, string path)
    {
      NewGraphicItem(path, graphicItem.Model, graphicItem.Shape, graphicItem.BoundingRect, graphicItem.Angle, graphicItem.FillColor, graphicItem.MirrorX, graphicItem.MirrorY);
    }

    public void NewGraphicItem(String path, String model, String shape, RectangleF boundingRect, Single angle, Color fillColor, bool mirrorX, bool mirrorY)
    {
      uint requestID;
      Guid guid;

      while (state.Exists("N_" + tempBoxKey.ToString()))
        tempBoxKey++;
      
      state.CreateGraphicItem(out requestID, out guid, "N_" + tempBoxKey.ToString(), path, model, shape, boundingRect, angle, fillColor, mirrorX, mirrorY);
    }

    public GraphicLink NewGraphicLink(GraphicLink graphicLink, float dx, float dy)
    {
      while (state.Exists("A_" + tempBoxKey.ToString()))
        tempBoxKey++;

      GraphicLink newGraphicLink = new GraphicLink("A_" + tempBoxKey.ToString());
      newGraphicLink.Destination = graphicLink.Destination;
      newGraphicLink.Origin = graphicLink.Origin;

      foreach (PointF point in graphicLink.controlPoints)
      {
        newGraphicLink.controlPoints.Add(new PointF(point.X + dx, point.Y + dy));
      }

      state.CreateLink(newGraphicLink, true, fcFlowChart);

      return newGraphicLink;
    }

    Box overBox;
    Arrow overArrow;

    private void fcFlowChart_Click(object sender, EventArgs e)
    {
      MouseEventArgs me = e as MouseEventArgs;
      overBox = fcFlowChart.GetBoxAt(fcFlowChart.ClientToDoc(me.Location), 2);
      overArrow = fcFlowChart.GetArrowAt(fcFlowChart.ClientToDoc(me.Location), 2);

      if (me.Button == MouseButtons.Left)
      {
        if ((overBox == null) && (overArrow == null)) // we're in free space...
        {
          if ((form1.barManager1.Commands["Mode.CreateNode"] as BarButtonCommand).Checked == true)
          {
            NewGraphicItem(
              state.CurrentPath,
              currentModelShape,
              currentGraphicShape,
              new RectangleF(fcFlowChart.ClientToDoc(me.Location), state.GraphicStencil(currentGraphicShape).defaultSize),
              0.0F,
              Color.LightBlue,
              false,
              false
              );
          }
          else
          {
            form1.Mode_Modify();
          }
        }
      }

      if (me.Button == MouseButtons.Right)
      {
        if (overBox != null)
        {
          ContextMenu boxMenu = new ContextMenu();
          boxMenu.MenuItems.Add("Unfinished");
          boxMenu.Show(fcFlowChart, me.Location);
          form1.Mode_Modify();
        }
        else if (overArrow != null)
        {
          ContextMenu arrowMenu = new ContextMenu();
          arrowMenu.MenuItems.Add("Route arrow", new EventHandler(RouteArrow));
          arrowMenu.Show(fcFlowChart, me.Location);
          form1.Mode_Modify();
        }
      }
    }

    private void RouteArrow(object sender, EventArgs e)
    {
      overArrow.Route();
    }

    private void fcFlowChart_BoxDeleting(object sender, BoxConfirmArgs e)
    {
      e.Confirm = false;
      state.Remove(e.Box.Tag as Item, fcFlowChart);
    }

    private void fcFlowChart_ArrowDeleting(object sender, ArrowConfirmArgs e)
    {
      e.Confirm = false;
      MessageBox.Show("Not Implemented");
    }

    private void fcFlowChart_ArrowCreated(object sender, ArrowEventArgs e)
    {
      tempArrowKey++;
      string newLinkTag = "A_" + tempArrowKey.ToString();
      e.Arrow.Text = newLinkTag;

      Box destinationBox = e.Arrow.Destination as Box;
      Box sourceBox = e.Arrow.Origin as Box;
      GraphicLink newGraphicLink = new GraphicLink(newLinkTag);

      if (destinationBox != null)
        newGraphicLink.Destination = (destinationBox.Tag as Item).Guid;

      if (sourceBox != null)
        newGraphicLink.Origin = (sourceBox.Tag as Item).Guid;

      newGraphicLink.controlPoints = new List<PointF>();
      foreach (PointF point in e.Arrow.ControlPoints)
      {
        newGraphicLink.controlPoints.Add(point);
      }

      state.CreateLink(newGraphicLink, true, fcFlowChart);
    }

    private void fcFlowChart_MouseDown(object sender, MouseEventArgs e)
    {
      overBox = fcFlowChart.GetBoxAt(fcFlowChart.ClientToDoc(e.Location), 2);
      overArrow = fcFlowChart.GetArrowAt(fcFlowChart.ClientToDoc(e.Location), 2);

      if (e.Button == MouseButtons.Left)
      {
        if ((overBox != null) || (overArrow != null)) // we're not in free space...
        {
          if ((form1.barManager1.Commands["Mode.CreateNode"] as BarButtonCommand).Checked == true)
          form1.Mode_Modify();
        }
      }
    }
  }
}
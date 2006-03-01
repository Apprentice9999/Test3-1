using MindFusion.FlowChartX;
namespace SysCAD.Editor
{
  partial class FrmFlowChart
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmFlowChart));
      this.fcFlowChart = new FlowChart();
      this.SuspendLayout();
      // 
      // fcFlowChart
      // 
      this.fcFlowChart.AllowDrop = true;
      this.fcFlowChart.AllowUnconnectedArrows = true;
      this.fcFlowChart.ArrowBaseSize = 1.5F;
      this.fcFlowChart.ArrowBrush = new SolidBrush("#FF000000");
      this.fcFlowChart.ArrowCrossings = ArrowCrossings.BreakOff;
      this.fcFlowChart.ArrowHandlesStyle = HandlesStyle.SquareHandles2;
      this.fcFlowChart.ArrowHead = ArrowHead.None;
      this.fcFlowChart.ArrowHeadSize = 1.5F;
      this.fcFlowChart.ArrowIntermSize = 1F;
      this.fcFlowChart.ArrowsRetainForm = true;
      this.fcFlowChart.ArrowsSnapToBorders = true;
      this.fcFlowChart.ArrowsSplittable = true;
      this.fcFlowChart.AutoSizeDoc = MindFusion.FlowChartX.AutoSize.AllDirections;
      this.fcFlowChart.Behavior = BehaviorType.Modify;
      this.fcFlowChart.BoxCustomDraw = CustomDraw.Additional2;
      this.fcFlowChart.BoxHandlesStyle = HandlesStyle.HatchHandles2;
      this.fcFlowChart.DefaultControlType = typeof(System.Windows.Forms.Button);
      this.fcFlowChart.DocExtents = ((System.Drawing.RectangleF)(resources.GetObject("fcFlowChart.DocExtents")));
      this.fcFlowChart.Dock = System.Windows.Forms.DockStyle.Fill;
      this.fcFlowChart.InplaceEditFont = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.fcFlowChart.IntermArrowHead = ArrowHead.Triangle;
      this.fcFlowChart.Location = new System.Drawing.Point(0, 0);
      this.fcFlowChart.Name = "fcFlowChart";
      this.fcFlowChart.PrintOptions.EnableShadows = false;
      this.fcFlowChart.RestrObjsToDoc = RestrictToDoc.NoRestriction;
      this.fcFlowChart.RoutingOptions.TriggerRerouting = RerouteArrows.WhenIntersectNode;
      this.fcFlowChart.Size = new System.Drawing.Size(772, 605);
      this.fcFlowChart.TabIndex = 0;
      this.fcFlowChart.ArrowModified += new ArrowMouseEvent(this.fcFlowChart_ArrowModified);
      this.fcFlowChart.MouseMove += new System.Windows.Forms.MouseEventHandler(this.fcFlowChart_MouseMove);
      this.fcFlowChart.BoxModified += new BoxMouseEvent(this.fcFlowChart_BoxModified);
      this.fcFlowChart.DrawBox += new BoxCustomDraw(this.fcFlowChart_DrawBox);
      this.fcFlowChart.ArrowModifying += new ArrowMouseEvent(this.fcFlowChart_ArrowModifying);
      // 
      // FrmFlowChart
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(772, 605);
      this.Controls.Add(this.fcFlowChart);
      this.Name = "FrmFlowChart";
      this.Text = "FlowChart";
      this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
      this.ResumeLayout(false);

    }

    #endregion

    public FlowChart fcFlowChart;
  }
}
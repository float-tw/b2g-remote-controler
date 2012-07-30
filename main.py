#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wx
import os
import socket

class MyFrame(wx.Frame):

	def __init__(self):
		wx.Frame.__init__(self, None, -1, "My Frame", size=(300, 500))
		panel = wx.Panel(self, -1)
		panel.Bind(wx.EVT_MOTION,  self.leave_pic)
		self.posText = wx.StaticText(panel, -1, "Pos:", pos=(10, 12))
		self.posCtrl = wx.TextCtrl(panel, -1, "", pos=(40, 10))
		self.touch_stage = wx.StaticText(panel, -1, "●", pos=(0, 12))
		self.touch_stage.SetForegroundColour('red')
		self.screenshot_button = wx.Button(panel, -1, "Get Screenshot")
		self.img = wx.StaticBitmap(panel, -1)#, self.screenshot)
		
		self.get_screenshot()

		self.img.Bind(wx.EVT_MOTION, self.OnMove)
		self.img.Bind(wx.EVT_LEFT_DOWN, self.touch)
		self.img.Bind(wx.EVT_LEFT_UP, self.touch)
		self.img.Bind(wx.EVT_TOOL_ENTER, self.touch)
		self.screenshot_button.Bind(wx.EVT_BUTTON, self.get_screenshot)

		self.vbox = wx.BoxSizer(wx.VERTICAL)
		self.hbox = wx.BoxSizer(wx.HORIZONTAL)

		self.hbox.Add(self.touch_stage, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		self.hbox.Add(self.posText, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		self.hbox.Add(self.posCtrl, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		self.hbox.Add(self.screenshot_button, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		
		self.vbox.Add(self.hbox, flag = wx.TOP | wx.ALIGN_CENTER_HORIZONTAL)
		self.vbox.AddSpacer(20)
		self.vbox.Add(self.img, flag = wx.ALIGN_CENTER_HORIZONTAL)

		panel.SetSizer(self.vbox)

	def OnMove(self, event):
		pos = event.GetPosition()
		self.posCtrl.SetValue("%s, %s" % (pos.x, pos.y))
		if event.LeftIsDown():
			self.touch_stage.SetForegroundColour('green')

	def load_screenshot(self, event):
		self.screenshot = wx.Image("screenshot1.png", wx.BITMAP_TYPE_ANY).ConvertToBitmap()
		wx.StaticBitmap(self, -1, self.screenshot, (10, 5), (self.screenshot.GetWidth(), self.screenshot.GetHeight()))
	
	def touch(self, event):
		event_type = event.GetEventType()
		#print event_type
		if event_type == wx.wxEVT_LEFT_DOWN:
			self.touch_stage.SetForegroundColour('green')
		elif event_type == wx.wxEVT_LEFT_UP:
			self.touch_stage.SetForegroundColour('red')
	
	def leave_pic(self, event):
		self.touch_stage.SetForegroundColour('red')
	
	def get_screenshot(self, event=0):
		print "get_screenshot"
		os.system("adb pull /dev/graphics/fb0 screenshotdata/fb0")
		os.system("dd bs=1920 count=800 if=screenshotdata/fb0 of=screenshotdata/fb0b")
		os.system("ffmpeg -vframes 1 -vcodec rawvideo -f rawvideo -pix_fmt rgb32 -s 480x800 -i screenshotdata/fb0b -f image2 -vcodec png screenshot.png")
		self.screenshot = wx.Image("screenshot.png", wx.BITMAP_TYPE_ANY)
		self.screenshot = self.screenshot.Scale(240, 400, wx.IMAGE_QUALITY_HIGH)
		self.screenshot = self.screenshot.ConvertToBitmap()
		self.img.SetBitmap(self.screenshot)

if __name__ == '__main__':
	app = wx.PySimpleApp()
	frame = MyFrame()
	frame.Show(True)
	app.MainLoop()


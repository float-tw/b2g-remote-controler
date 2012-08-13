#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wx
import os
import socket

class MyFrame(wx.Frame):

	def __init__(self):
		wx.Frame.__init__(self, None, -1, "My Frame", size=(300, 500))
		port = 2222
		panel = wx.Panel(self, -1)
		panel.Bind(wx.EVT_MOTION,  self.leave_pic)
		self.Bind(wx.EVT_CLOSE, self.OnClose)
		self.posText = wx.StaticText(panel, -1, "Pos:", pos=(10, 12))
		self.posCtrl = wx.TextCtrl(panel, -1, "", pos=(40, 10))
		self.touch_stage_text = wx.StaticText(panel, -1, "●", pos=(0, 12))
		self.touch_stage_text.SetForegroundColour('red')
		self.touch_stage = False
		self.screenshot_button = wx.Button(panel, -1, "Get Screenshot")
		self.img = wx.StaticBitmap(panel, -1)
		
		self.get_screenshot()

		self.img.Bind(wx.EVT_MOTION, self.OnMove)
		self.img.Bind(wx.EVT_LEFT_DOWN, self.touch)
		self.img.Bind(wx.EVT_LEFT_UP, self.touch)
		self.img.Bind(wx.EVT_TOOL_ENTER, self.touch)
		self.screenshot_button.Bind(wx.EVT_BUTTON, self.get_screenshot)

		self.vbox = wx.BoxSizer(wx.VERTICAL)
		self.hbox = wx.BoxSizer(wx.HORIZONTAL)

		self.hbox.Add(self.touch_stage_text, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		self.hbox.Add(self.posText, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		self.hbox.Add(self.posCtrl, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		self.hbox.Add(self.screenshot_button, border=5, flag = wx.ALIGN_CENTER_VERTICAL)
		
		self.vbox.Add(self.hbox, flag = wx.TOP | wx.ALIGN_CENTER_HORIZONTAL)
		self.vbox.AddSpacer(20)
		self.vbox.Add(self.img, flag = wx.ALIGN_CENTER_HORIZONTAL)

		panel.SetSizer(self.vbox)

		# run target progarm
		os.system("adb shell /data/remote_server &");
		os.system("adb forward tcp:{0} tcp:{0}".format(port))

		# connect
		self.sock = socket.socket()
		try:
			self.sock.connect(("localhost", port))
		except socket.error, msg:
			print msg
			dlg = wx.MessageDialog(self,
				msg,
				"Error", wx.OK)
			dlg.ShowModal()
			dlg.Destroy()

	def OnClose(self, event):
		try:
			self.sock.send("-1 -1\n")
			time.sleep(0.5)
			self.sock.close()
		except:
			pass
		self.Destroy()


	def OnMove(self, event):
		pos = event.GetPosition()
		self.posCtrl.SetValue("%s, %s" % (pos.x, pos.y))
		if self.touch_stage:
			self.send_touch_event(pos)

	def touch(self, event):
		event_type = event.GetEventType()
		if event_type == wx.wxEVT_LEFT_DOWN:
			self.touch_stage_text.SetForegroundColour('green')
			self.touch_stage = True
			self.send_touch_event( event.GetPosition() )
		elif event_type == wx.wxEVT_LEFT_UP:
			self.touch_stage_text.SetForegroundColour('red')
			self.touch_stage = False
			self.sock.send("\n")
	
	def send_touch_event(self, pos):
		self.sock.send("{0} {1}\n".format(pos.x*2, pos.y*2))
	
	def leave_pic(self, event):
		self.touch_stage_text.SetForegroundColour('red')
		self.touch_stage = False

	def get_screenshot(self, event=0):
		print "get_screenshot"
		os.system("adb shell /data/gsnap /data/screenshot.jpeg /dev/graphics/fb0")
		os.system("adb pull /data/screenshot.jpeg")
		self.screenshot = wx.Image("screenshot.jpeg", wx.BITMAP_TYPE_ANY)
		self.screenshot = self.screenshot.Scale(240, 400, wx.IMAGE_QUALITY_HIGH)
		self.screenshot = self.screenshot.ConvertToBitmap()
		self.img.SetBitmap(self.screenshot)

if __name__ == '__main__':
	app = wx.PySimpleApp()
	frame = MyFrame()
	frame.Show(True)
	app.MainLoop()


import { BindingEngine } from 'aurelia-binding'
import { ReconnectingWebsocket } from './reconnecting-websocket'
import { GameboyCanvasCustomElement } from './components/gameboy-canvas'

export class App {
  static inject = [BindingEngine]
  constructor(BindingEngine) {
    this.myCanvas = HTMLCanvasElement
    this.BindingEngine = BindingEngine
    this.message = 'Hello World!'
    this.messageData = 'Data go here'
    this.socket = new ReconnectingWebsocket('ws://localhost:9500')
    this.socket.setOnOpen((str) => {
      console.log(str)
    })
    this.socket.setOnMessage((messageData) => {
      this.messageData = messageData
    })
  }

  bind() {
  }

  colorCanvas() {
    this.canvas.redraw()
  }

  resizeCanvas() {
    this.canvas.resize()
  }

  reset() {
    this.canvas.reset()
  }

  step() {
    this.sendMessage('step')
  }

  restartGame() {
    this.sendMessage('reset')
  }

  startGame() {
    this.sendMessage('start')
  }

  stopGame() {
    this.sendMessage('stop')
  }

  sendMessage(message) {
    this.socket.socket.send(message)
  }
}

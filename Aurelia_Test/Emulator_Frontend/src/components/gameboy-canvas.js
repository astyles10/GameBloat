import { customElement, inlineView } from 'aurelia-framework'

@customElement('gameboy-canvas')
@inlineView('<template><canvas element.ref="element" style="border:1px solid #000000"></canvas></template>')
export class GameboyCanvasCustomElement {
  constructor() {
    this.screen = {}
  }

  attached() {
    this.canvas = this.element.getContext('2d')
  }

  reset() {
    if (this.canvas.createImageData)
      this.screen = this.canvas.createImageData(160, 144)
    else if (this.canvas.getImageData)
      this.screen = this.canvas.getImageData(0, 0, 160, 144)
    else
      this.screen = {
        width: 160,
        height: 144,
        data: new Array(160 * 144 * 4)
      }

    this.element.width = this.screen.width
    this.element.height = this.screen.height

    this.screen.data.fill(255)
    this.canvas.putImageData(this.screen, 0, 0)
  }

  resize() {
    this.element.width += 10
    this.element.height += 10
  }

  redraw() {
    this.canvas.beginPath()
    this.canvas.rect(0, 0, this.element.width, this.element.height)
    let color = (this.canvas.fillStyle === '#ff0000' ? 'blue' : 'red')
    this.canvas.fillStyle = color
    this.canvas.fill()
  }
}
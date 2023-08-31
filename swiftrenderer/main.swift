//
//  main.swift
//  swiftrenderer
//
//  Created by john on 2022/12/27.
//

import Foundation

func goRender() {
//    let filePath = "/Users/john/Project/Personal/swiftrenderer/swiftrenderer/resources/diablo3_pose_diffuse.tga"
//    let image = TGAImage.load(filePath: filePath)
//    print(image)
    let color = TGARGBColor(r: 255, g: 255, b: 255, a: 255)
    let image = TGAImage(width: 100, height: 100, colorFormat: .rgb)
    image.set(x: 10, y: 10, color: color)
    image.write(filePath: "file:///Users/john/Project/Personal/swiftrenderer/swiftrenderer/resources/output/test.tga", runLengthEncode: false)
}

goRender()

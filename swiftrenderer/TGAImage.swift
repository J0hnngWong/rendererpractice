//
//  TGAImage.swift
//  swiftrenderer
//
//  Created by john on 2022/12/27.
//

import Foundation

// Reference: https://en.wikipedia.org/wiki/Truevision_TGA

protocol TGAColor {
    var bytespp: Int { get }
    var data: [UInt8] { get }
}

struct TGARGBColor: TGAColor {
    var bytespp: Int {
        TGAImage.ColorFormat.rgba.rawValue
    }
    
    var data: [UInt8] {
        [b, g, r, a]
    }
    
    var r: UInt8
    var g: UInt8
    var b: UInt8
    var a: UInt8
}

struct TGAImageHeader {
    let idLength: UInt8
    let colorMapType: UInt8
    let imageType: UInt8
    // color map specification
    let firstEntryIndex: UInt16
    let colorMapLength: UInt16
    let colorMapEntrySize: UInt8
    // image specification
    let originX: UInt16
    let originY: UInt16
    let imageWidth: UInt16
    let imageHeight: UInt16
    let pixelDepth: UInt8
    let imageDescriptor: UInt8
}

class TGAImage {
    
    enum TGAImageError: Error {
        case readFail
    }
    
    enum ColorFormat: Int {
        case grayScale = 1
        case rgb = 3
        case rgba = 4
    }
    
    var canvasData: Data
    let width: Int
    let height: Int
    let colorFormat: ColorFormat
    
    init(width: Int, height: Int, colorFormat: ColorFormat) {
        self.width = width
        self.height = height
        self.colorFormat = colorFormat
        canvasData = Data(repeating: 0, count: width * height * colorFormat.rawValue)
    }
    
//    private init(filePath: String) {
//        do {
//            guard let fileData = FileManager.default.contents(atPath: filePath) else { throw TGAImageError.readFail }
//            data = fileData
//        } catch let err {
//            fatalError(err.localizedDescription)
//        }
//    }
    
    func set(x: Int, y: Int, color: TGAColor) {
//        canvasData.replaceSubrange((width * y + x)..<(width * y + x + color.bytespp), with: color.data, count: color.bytespp)
        for (index, colorByte) in color.data.enumerated() {
            canvasData[width * y + x + index] = colorByte
        }
    }
    
    func write(filePath: String) {
        
    }
}

extension TGAImage {
//    static func load(filePath: String) -> TGAImage {
//        TGAImage(filePath: filePath)
//    }
}

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
    /*
     0 no image data is present
     1 uncompressed color-mapped image
     2 uncompressed true-color image
     3 uncompressed black-and-white (grayscale) image
     9 run-length encoded color-mapped image
     10 run-length encoded true-color image
     11 run-length encoded black-and-white (grayscale) image
     */
    let imageType: UInt8
    // color map specification
    /*
     First entry index (2 bytes): index of first color map entry that is included in the file
     Color map length (2 bytes): number of entries of the color map that are included in the file
     Color map entry size (1 byte): number of bits per color map entry
     */
    let firstEntryIndex: (UInt8, UInt8) //2
    let colorMapLength: (UInt8, UInt8) //2
    let colorMapEntrySize: UInt8
    // image specification
    let originX: (UInt8, UInt8) //2
    let originY: (UInt8, UInt8) //2
    let imageWidth: (UInt8, UInt8) //2
    let imageHeight: (UInt8, UInt8) //2
    let pixelDepth: UInt8
    /// Image descriptor (1 byte): bits 3-0 give the alpha channel depth, bits 5-4 give pixel ordering
    let imageDescriptor: UInt8
    
    func headerSet() -> [UInt8] {
        [
            idLength,
            colorMapType,
            imageType,
            firstEntryIndex.0,
            firstEntryIndex.1,
            colorMapLength.0,
            colorMapLength.1,
            colorMapEntrySize,
            originX.0,
            originX.1,
            originY.0,
            originY.1,
            imageWidth.0,
            imageWidth.1,
            imageHeight.0,
            imageHeight.1,
            pixelDepth,
            imageDescriptor
        ]
    }
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
    
    func write(filePath: String, runLengthEncode: Bool = true) {
        guard let fileURL = URL(string: filePath) else { return }
        var outData = Data()
        let developerAreaRef = [UInt8](repeating: 0, count: 4)
        let extensionAreaRef = [UInt8](repeating: 0, count: 4)
        let endPoint = Character("\0")
        let footerList = "TRUEVISION-XFILE.\(endPoint)".compactMap({ $0.asciiValue })
        // header
        outData.append(contentsOf: generateHeader(runLengthEncode: runLengthEncode).headerSet())
        if !runLengthEncode {
            outData.append(canvasData)
        }
        outData.append(contentsOf: developerAreaRef)
        outData.append(contentsOf: extensionAreaRef)
        outData.append(contentsOf: footerList)
        do {
            try outData.write(to: fileURL)
        } catch let error {
            fatalError("Fail to Write to File: \(filePath), error: \(error)")
        }
    }
}

extension TGAImage {
    func generateHeader(runLengthEncode: Bool = true) -> TGAImageHeader {
        let header = TGAImageHeader(
            idLength: 0,
            colorMapType: 0,
            imageType: colorFormat == .grayScale ? (runLengthEncode ? 11 : 3) : (runLengthEncode ? 10 : 2),
            firstEntryIndex: (0, 0),
            colorMapLength: (0, 0),
            colorMapEntrySize: 0,
            originX: (0, 0),
            originY: (0, 0),
            // width大于uint8可存储范围后会出问题，后续解决
            imageWidth: (UInt8(width), UInt8(UInt16(width) >> 8)), //(UInt8(UInt16(width) >> 8), UInt8(width)),
            imageHeight: (UInt8(height), UInt8(UInt16(height) >> 8)), //(UInt8(UInt16(height) >> 8), UInt8(height)),
            pixelDepth: UInt8(self.colorFormat.rawValue << 3),
            imageDescriptor: 0x20) // top-left origin
        return header
    }
}

extension TGAImage {
//    static func load(filePath: String) -> TGAImage {
//        TGAImage(filePath: filePath)
//    }
}

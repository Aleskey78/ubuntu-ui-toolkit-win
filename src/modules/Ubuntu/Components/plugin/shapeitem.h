/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Loïc Molinari <loic.molinari@canonical.com>
 */

#ifndef UBUNTU_COMPONENTS_SHAPE_H
#define UBUNTU_COMPONENTS_SHAPE_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QSGNode>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QtQuick/qsgtexture.h>
#include <QtGui/QOpenGLFunctions>

// QtQuick item.

class ShapeItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(HAlignment)
    Q_ENUMS(VAlignment)
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY baseColorChanged)
    Q_PROPERTY(QColor gradientColor READ gradientColor WRITE setGradientColor
               NOTIFY gradientColorChanged)
    Q_PROPERTY(QString radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QVariant image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(bool stretched READ stretched WRITE setStretched NOTIFY stretchedChanged)
    Q_PROPERTY(HAlignment horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY(VAlignment verticalAlignment READ verticalAlignment WRITE setVerticalAlignment NOTIFY verticalAlignmentChanged)
    Q_PROPERTY(QString borderSource READ borderSource WRITE setBorderSource NOTIFY borderSourceChanged)

public:
    ShapeItem(QQuickItem* parent=0);

    enum Radius { SmallRadius, MediumRadius };
    enum Border { RawBorder, IdleBorder, PressedBorder };
    enum HAlignment { AlignLeft = 0, AlignHCenter = 1, AlignRight = 2 };
    enum VAlignment { AlignTop = 0, AlignVCenter = 1, AlignBottom = 2 };

    QColor baseColor() const { return baseColor_; }
    void setBaseColor(const QColor& baseColor);
    QColor gradientColor() const { return gradientColor_; }
    void setGradientColor(const QColor& gradientColor);
    QString radius() const { return radiusString_; }
    void setRadius(const QString& radius);
    QString borderSource() const { return borderSource_; }
    void setBorderSource(const QString& borderSource);
    QVariant image() const { return QVariant::fromValue(image_); }
    void setImage(QVariant image);
    bool stretched() const { return stretched_; }
    void setStretched(bool stretched);
    HAlignment horizontalAlignment() const { return hAlignment_; }
    void setHorizontalAlignment(HAlignment horizontalAlignment);
    VAlignment verticalAlignment() const { return vAlignment_; }
    void setVerticalAlignment(VAlignment verticalAlignment);
    Q_SLOT void gridUnitChanged();

Q_SIGNALS:
    void baseColorChanged();
    void gradientColorChanged();
    void radiusChanged();
    void borderChanged();
    void imageChanged();
    void stretchedChanged();
    void horizontalAlignmentChanged();
    void verticalAlignmentChanged();
    void borderSourceChanged();

protected:
    virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);
    virtual QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

private:
    enum DirtyFlags {
        NotDirty           = 0,
        DirtyBaseColor     = (1 << 0),
        DirtyGradientColor = (1 << 1),
        DirtyRadius        = (1 << 2),
        DirtyBorder        = (1 << 3),
        DirtyImage         = (1 << 4),
        DirtyStretched     = (1 << 5),
        DirtyHAlignment    = (1 << 6),
        DirtyVAlignment    = (1 << 7),
        DirtyGridUnit      = (1 << 8),
        DirtyGeometry      = (1 << 9),
        DirtyAll           = (DirtyBaseColor | DirtyGradientColor | DirtyRadius | DirtyBorder
                              | DirtyImage | DirtyStretched | DirtyHAlignment | DirtyVAlignment
                              | DirtyGridUnit | DirtyGeometry)
    };

    QColor baseColor_;
    QColor gradientColor_;
    QString radiusString_;
    Radius radius_;
    QString borderSource_;
    Border border_;
    QQuickItem* image_;
    bool stretched_;
    HAlignment hAlignment_;
    VAlignment vAlignment_;
    float gridUnit_;
    QRectF geometry_;
    QFlags<DirtyFlags> dirtyFlags_;

    Q_DISABLE_COPY(ShapeItem)
};

// Scene graph textured material.

class ShapeTexturedMaterial : public QSGMaterial
{
public:
    ShapeTexturedMaterial();
    virtual QSGMaterialType* type() const;
    virtual QSGMaterialShader* createShader() const;
    QSGTextureProvider* imageTextureProvider() const;
    void setImage(QQuickItem* image);
    QSGTexture* shapeTexture() const { return shapeTexture_; }
    QSGTexture::Filtering filtering() const { return filtering_; }
    void setShapeTexture(QSGTexture* shapeTexture, bool scaledDown);

private:
    QSGTextureProvider* imageTextureProvider_;
    QSGTexture* shapeTexture_;
    QSGTexture::Filtering filtering_;
};

// Scene graph textured material shader.

class ShapeTexturedShader : public QSGMaterialShader
{
public:
    virtual char const* const* attributeNames() const;
    virtual void initialize();
    virtual void updateState(
        const RenderState& state, QSGMaterial* newEffect, QSGMaterial* oldEffect);

private:
    virtual const char* vertexShader() const;
    virtual const char* fragmentShader() const;

    int matrixId_;
    int opacityId_;
    QOpenGLFunctions* glFuncs_;
};

// Scene graph colored material.

class ShapeColoredMaterial : public QSGMaterial
{
public:
    ShapeColoredMaterial();
    virtual QSGMaterialType* type() const;
    virtual QSGMaterialShader* createShader() const;
    const QVector4D& baseColor() const { return baseColor_; }
    void setBaseColor(const QColor& baseColor);
    const QVector4D& gradientColor() const { return gradientColor_; }
    void setGradientColor(const QColor& gradientColor);
    QSGTexture* shapeTexture() const { return shapeTexture_; }
    QSGTexture::Filtering filtering() const { return filtering_; }
    void setShapeTexture(QSGTexture* shapeTexture, bool scaledDown);

private:
    QVector4D baseColor_;
    QVector4D gradientColor_;
    QSGTexture* shapeTexture_;
    QSGTexture::Filtering filtering_;
};

// Scene graph colored material shader.

class ShapeColoredShader : public QSGMaterialShader
{
public:
    virtual char const* const* attributeNames() const;
    virtual void initialize();
    virtual void updateState(
        const RenderState& state, QSGMaterial* newEffect, QSGMaterial* oldEffect);

private:
    virtual const char* vertexShader() const;
    virtual const char* fragmentShader() const;

    int matrixId_;
    int opacityId_;
    int baseColorId_;
    int gradientColorId_;
};

// Scene graph node.

struct TextureData;

class ShapeNode : public QObject, public QSGGeometryNode
{
    Q_OBJECT

public:
    struct Vertex {
        float position[2];
        float shapeCoordinate[2];
        float imageCoordinate[2];
        float padding[2];  // Ensure a 32 bytes stride.
    };
    enum MaterialType { TexturedMaterial, ColoredMaterial };

    ShapeNode(ShapeItem* item);
    ShapeTexturedMaterial* texturedMaterial() { return &texturedMaterial_; }
    ShapeColoredMaterial* coloredMaterial() { return &coloredMaterial_; }
    void setVertices(const QRectF& geometry, float radius, QQuickItem* image, bool stretched,
                     ShapeItem::HAlignment hAlignment, ShapeItem::VAlignment vAlignment,
                     float shapeCoordinate[][2]);
    void setMaterialType(MaterialType material);

private:
    ShapeItem* item_;
    QSGGeometry geometry_;
    ShapeTexturedMaterial texturedMaterial_;
    ShapeColoredMaterial coloredMaterial_;
    MaterialType currentMaterial_;
};

QML_DECLARE_TYPE(ShapeItem)

#endif // UBUNTU_COMPONENTS_SHAPE_H

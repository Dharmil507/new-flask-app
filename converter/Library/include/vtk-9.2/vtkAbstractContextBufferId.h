/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkAbstractContextBufferId.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**
 * @class   vtkAbstractContextBufferId
 * @brief   2D array of ids, used for picking.
 *
 *
 * An 2D array where each element is the id of an entity drawn at the given
 * pixel. The access is not specified in this class.
 * The effective/concrete subclass vtkContextBufferId stores the whole buffer
 * in RAM. The access to a value is fast and independent of the OpenGL.
 * However it requires to first fill the whole buffer by transferring the
 * buffer generated by OpenGL from the VRAM to the RAM. It is inefficient if
 * the context of the scene changes during interaction.
 *
 * The effective/concrete subclass vtkOpenGLContextBufferId keeps the buffer id
 * on the VRAM in a texture image. The access to a value is slower than a
 * simple read access to an array but it does not require a large transfer of
 * data from the VRAM to the RAM.
 *
 * @sa
 * vtkContextBufferId, vtkOpenGLContextBufferId
 */

#ifndef vtkAbstractContextBufferId_h
#define vtkAbstractContextBufferId_h

#include "vtkObject.h"
#include "vtkRenderingContext2DModule.h" // For export macro

class vtkRenderWindow;

class VTKRENDERINGCONTEXT2D_EXPORT vtkAbstractContextBufferId : public vtkObject
{
public:
  vtkTypeMacro(vtkAbstractContextBufferId, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  static vtkAbstractContextBufferId* New();

  ///@{
  /**
   * Number of columns. Initial value is 0.
   */
  vtkGetMacro(Width, int);
  ///@}

  ///@{
  /**
   * Set the number of columns. Initial value is 0.
   */
  vtkSetMacro(Width, int);
  ///@}

  ///@{
  /**
   * Number of rows. Initial value is 0.
   */
  vtkGetMacro(Height, int);
  ///@}

  ///@{
  /**
   * Set the number of rows. Initial value is 0.
   */
  vtkSetMacro(Height, int);
  ///@}

  ///@{
  /**
   * Set/Get the OpenGL context owning the texture object resource.
   */
  virtual void SetContext(vtkRenderWindow* context) = 0;
  virtual vtkRenderWindow* GetContext() = 0;
  ///@}

  /**
   * Returns if the context supports the required extensions.
   * \pre context_is_set: this->GetContext()!=0
   */
  virtual bool IsSupported() = 0;

  /**
   * Allocate the memory for at least Width*Height elements.
   * \pre positive_width: GetWidth()>0
   * \pre positive_height: GetHeight()>0
   */
  virtual void Allocate() = 0;

  /**
   * Tell if the buffer has been allocated.
   */
  virtual bool IsAllocated() const = 0;

  /**
   * Copy the contents of the current read buffer to the internal structure
   * starting at lower left corner of the framebuffer (srcXmin,srcYmin).
   * \pre is_allocated: this->IsAllocated()
   */
  virtual void SetValues(int srcXmin, int srcYmin) = 0;

  /**
   * Return item under abscissa x and ordinate y.
   * Abscissa go from left to right.
   * Ordinate go from bottom to top.
   * The return value is -1 if there is no item.
   * \pre is_allocated: IsAllocated()
   * \post valid_result: result>=-1
   */
  virtual vtkIdType GetPickedItem(int x, int y) = 0;

  /**
   * Release any graphics resources that are being consumed by this object.
   * Default implementation is empty.
   */
  virtual void ReleaseGraphicsResources();

protected:
  vtkAbstractContextBufferId();
  ~vtkAbstractContextBufferId() override;

  int Width;
  int Height;

private:
  vtkAbstractContextBufferId(const vtkAbstractContextBufferId&) = delete;
  void operator=(const vtkAbstractContextBufferId&) = delete;
};

#endif // #ifndef vtkAbstractContextBufferId_h

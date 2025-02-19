#include <memory>
#include <string>
#include <utility>

#include "reference_column.hpp"

namespace opossum {

ReferenceColumn::ReferenceColumn(const std::shared_ptr<const Table> referenced_table, const size_t referenced_column_id,
                                 const std::shared_ptr<const PosList> pos)
    : _referenced_table(referenced_table), _referenced_column_id(referenced_column_id), _pos_list(pos) {
  if (IS_DEBUG) {
    auto referenced_column = _referenced_table->get_chunk(0).get_column(referenced_column_id);
    auto reference_col = std::dynamic_pointer_cast<ReferenceColumn>(referenced_column);
    if (reference_col != nullptr) {
      // cast was successful, but was expected to fail
      throw std::logic_error("referenced_column must not be a ReferenceColumn");
    }
  }
}

const AllTypeVariant ReferenceColumn::operator[](const size_t i) const {
  auto chunk_info = _referenced_table->locate_row((*_pos_list).at(i));
  auto &chunk = _referenced_table->get_chunk(chunk_info.first);

  return (*chunk.get_column(_referenced_column_id))[chunk_info.second];
}

void ReferenceColumn::append(const AllTypeVariant &) { throw std::logic_error("ReferenceColumn is immutable"); }

const std::shared_ptr<const PosList> ReferenceColumn::pos_list() const { return _pos_list; }
const std::shared_ptr<const Table> ReferenceColumn::referenced_table() const { return _referenced_table; }
size_t ReferenceColumn::referenced_column_id() const { return _referenced_column_id; }

size_t ReferenceColumn::size() const { return _pos_list->size(); }
void ReferenceColumn::visit(ColumnVisitable &visitable, std::shared_ptr<ColumnVisitableContext> context) {
  visitable.handle_reference_column(*this, std::move(context));
}

// writes the length and value at the chunk_offset to the end off row_string
void ReferenceColumn::write_string_representation(std::string &row_string, const ChunkOffset chunk_offset) const {
  // retrieving the chunk_id for the given chunk_offset
  auto chunk_info = _referenced_table->locate_row((*_pos_list).at(chunk_offset));
  // call the equivalent function of the referenced value column
  _referenced_table->get_chunk(chunk_info.first)
      .get_column(_referenced_column_id)
      ->write_string_representation(row_string, chunk_offset);
}

}  // namespace opossum
